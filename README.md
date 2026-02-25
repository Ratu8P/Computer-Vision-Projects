[root@sjac pcie]# cat check_aer_read_only.sh
#!/usr/bin/bash

# trap ctrl-c and call ctrl_c()
trap ctrl_c INT

function ctrl_c() {
        printf "\n\nInterrupted at iteration: %d\n" $cnt
        printf "Total EP AER error count: %d\n" $ep_aer_err_cnt
        printf "Total EP UAER error count: %d\n" $ep_uaer_err_cnt
        printf "Total RC AER error count: %d\n" $rc_aer_err_cnt
        printf "Total RC UAER error count: %d\n" $rc_uaer_err_cnt
        printf "Total EP lane error count: %d\n" $lane_error_cnt
        printf "Total EP link down error count: %d\n" $link_down_err_cnt
        return 0
}

rate_decode () {
        if [ $1 == "gen5" ]; then
                return_val=4
        elif [ $1 == "gen4" ]; then
                return_val=3
        elif [ $1 == "gen3" ]; then
                return_val=2
        elif [ $1 == "gen2" ]; then
                return_val=1
        elif [ $1 == "gen1" ]; then
                return_val=0
        else
                return_val=10
                return
        fi
}

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 BDF rate_string count. Ex: $0 01:00.0 10000"
    echo "Usage: Where BDF is in the form of xx:xx.x "
    return
fi



ep_bdf=$1
iterations=$2
RATE=("Invalid" "gen1" "gen2" "gen3" "gen4" "gen5")
return_val=0
ep_aer_err_cnt=0
ep_uaer_err_cnt=0
rc_aer_err_cnt=0
rc_uaer_err_cnt=0
link_down_err_cnt=0
rate_change_err_cnt=0
lane_error_cnt=0
width_error_cnt=0
error_detected=0
how_often=${3:-1000}

if [ -z "$ep_bdf" ]; then
        echo "Error: no device specified"
        return 1
fi

if [ ! -e "/sys/bus/pci/devices/$ep_bdf" ]; then
        ep_bdf="0000:$ep_bdf"
fi

if [ ! -e "/sys/bus/pci/devices/$ep_bdf" ]; then
        echo "Error: device $ep_bdf not found"
        return 1
fi

echo "$ep_bdf=/sys/bus/pci/devices/$ep_bdf"
rc_bdf=$(basename $(dirname $(readlink "/sys/bus/pci/devices/$ep_bdf")))
echo "rc_bdf=$rc_bdf ep_bdf=$ep_bdf"

link_status=$(setpci -s $ep_bdf CAP_EXP+12.W)
initial_rate=$(("0x$link_status" & 0xF))
initial_width=$((("0x$link_status" & 0xF0) >> 4))

printf "Running with initial state of ${RATE[initial_rate]}x${initial_width}\n"

# Clear
aer=$(setpci -s $ep_bdf ECAP_AER+10.L)
aer=$(("0x$aer" & 0xFFFF))
if [[ $aer -ne 0 ]]; then
        #setpci -s $ep_bdf ECAP_AER+10.L=$aer
        printf "Clearing initial EP AER error: aer=%s\n" $aer
fi

uaer=$(setpci -s $ep_bdf ECAP_AER+4.L)
uaer=$(("0x$uaer" & 0xFFFF))
if [[ $uaer -ne 0 ]]; then
        #setpci -s $ep_bdf ECAP_AER+4.L=$uaer
        printf "Clearing initial EP UAER error: aer=%s\n" $uaer
fi

lane_error=$(setpci -s $ep_bdf ECAP_SECPCI+8.l)
lane_error=$(("0x$lane_error" & 0xFF))
#setpci -s $ep_bdf ECAP_SECPCI+8.w=$lane_error
if [[ $lane_error -ne 0 ]]; then
        printf "Clearing initial EP lane error: lane_error=0x%x\n" $lane_error
fi

aer=$(setpci -s $rc_bdf ECAP_AER+10.L)
aer=$(("0x$aer" & 0xFFFF))
#setpci -s $rc_bdf ECAP_AER+10.L=$aer
if [[ $aer -ne 0 ]]; then
        printf "Clearing initial RC AER error: aer=%s\n" $aer
fi

uaer=$(setpci -s $rc_bdf ECAP_AER+4.L)
uaer=$(("0x$uaer" & 0xFFFF))
if [[ $uaer -ne 0 ]]; then
        setpci -s $rc_bdf ECAP_AER+4.L=$uaer
        printf "Clearing initial RC UAER error: aer=%s\n" $uaer
fi


for (( cnt=1; cnt<=$iterations; cnt++ ))
do
        if [ $(expr $cnt % $how_often) == "0" ]; then
                if [[ $error_detected -eq 0 ]]; then
                        printf "Done %8d iterations of %d so far...\n" $cnt $iterations
                fi
                error_detected=0
        fi

        aer=$(setpci -s $ep_bdf ECAP_AER+10.L)
        aer=$(("0x$aer" & 0xFFFF))
        if [[ $aer -ne 0 ]]; then
                error_detected=1
                ep_aer_err_cnt=$((ep_aer_err_cnt +1))
                #setpci -s $ep_bdf ECAP_AER+10.L=$aer
                printf "ERROR: EP aer=%s iteration=%d\n" $aer $cnt
        fi

        uaer=$(setpci -s $ep_bdf ECAP_AER+4.L)
        uaer=$(("0x$uaer" & 0xFFFF))
        if [[ $uaer -ne 0 ]]; then
                error_detected=1
                ep_uaer_err_cnt=$((ep_uaer_err_cnt +1))
                #setpci -s $ep_bdf ECAP_AER+4.L=$uaer
                printf "ERROR: EP uncorrectable error=%s iteration=%d\n" $uaer $cnt
        fi

        aer=$(setpci -s $rc_bdf ECAP_AER+10.L)
        if [[ $aer -ne 0 ]]; then
                error_detected=1
                rc_aer_err_cnt=$((ep_aer_err_cnt +1))
                #setpci -s $ep_bdf ECAP_AER+10.L=$aer
                printf "ERROR: RC aer=%s iteration=%d\n" $aer $cnt
        fi

        uaer=$(setpci -s $rc_bdf ECAP_AER+4.L)
        if [[ $uaer -ne 0 ]]; then
                error_detected=1
                rc_uaer_err_cnt=$((rc_uaer_err_cnt +1))
                #setpci -s $rc_bdf ECAP_AER+4.L=$uaer
                printf "ERROR: RC uncorrectable error=%s iteration=%d\n" $uaer $cnt
        fi

        # ltssm_org=$(setpci -s $ep_bdf 728.L)
        # ltssm=$(("0x$ltssm_org" & 0x3F))
        # if [[ $ltssm -ne 0x11 ]]; then
        #       link_down_err_cnt=$((link_down_err_cnt + 1))
        #       printf "ERROR: link is down. Wait 2 seconds for recovery\n" $ltssm_org
        #       sleep 2
        #       ltssm_org=$(setpci -s $ep_bdf 728.L)
        #       ltssm=$(("0x$ltssm_org" & 0x3F))
        #       if [[ $ltssm -ne 0x11 ]]; then
        #               printf "ERROR: link is stil down after 2 seconds: %s Stop checking AER" $ltssm_org
        #               return
        #       fi
        # fi

        lane_error=$(setpci -s $ep_bdf ECAP_SECPCI+8.w)
        lane_error=$(("0x$lane_error" & 0xFF))
        if [[ $lane_error -ne 0 ]]; then
                error_detected=1
                lane_error_cnt=$((lane_error_cnt + 1))
                printf "ERROR: EP lane_error=0x%x iteration=%d\n" $lane_error $cnt
                #setpci -s $ep_bdf ECAP_SECPCI+8.w=$lane_error
        fi

        link_status=$(setpci -s $ep_bdf CAP_EXP+12.W)
        rate=$(("0x$link_status" & 0xF))
        width=$((("0x$link_status" & 0xF0) >> 4))
        if [[ $rate -ne $initial_rate ]]; then
                error_detected=1
                printf "ERROR: rate has change at iteration $cnt: Initial rate ${RATE[initial_rate]}, current rate ${RATE[rate]}.\n"
                printf "ERROR: Waiting for 2 seconds to see if rate recover"
        sleep 2
        link_status=$(setpci -s $ep_bdf CAP_EXP+12.W)
        rate=$(("0x$link_status" & 0xF))
                if [[ $rate -ne $initial_rate ]]; then
                        printf "ERROR: rate has not recovered: Initial rate ${RATE[initial_rate]}, current rate ${RATE[rate]}.\n"
                        printf "Stop checking AER\n"
                        return 1
                fi
        fi
        link_status=$(setpci -s $ep_bdf CAP_EXP+12.W)
        width=$((("0x$link_status" & 0xF0) >> 4))
        if [[ $width -ne $initial_width ]]; then
                error_detected=1
                printf "ERROR: width has change at iteration $cnt: Initial width $initial_width, current width $width.\n"
        fi
done

printf "Total EP AER error count: %d\n" $ep_aer_err_cnt
printf "Total EP UAER error count: %d\n" $ep_uaer_err_cnt
printf "Total RC AER error count: %d\n" $rc_aer_err_cnt
printf "Total RC UAER error count: %d\n" $rc_uaer_err_cnt
printf "Total EP lane error count: %d\n" $lane_error_cnt
printf "Total EP link down error count: %d\n" $link_down_err_cnt

2026-02-25 17:00:20,152 | [hotreset_test] | BROADCAST | INFO | ========Cycle | Total Cycle: 306 | 1000========
2026-02-25 17:00:20,153 | [hotreset_test] | 0x0:98:00.00 | INFO | RC Secondary Bus Reset bit was set.
2026-02-25 17:00:20,156 | [hotreset_test] | 0x0:98:00.00 | DEBUG | Clear RC Slot Status Register by writing 0x0108.
2026-02-25 17:00:20,156 | [hotreset_test] | 0x0:98:00.00 | INFO | RC Secondary Bus Reset bit was reset.
2026-02-25 17:00:20,156 | [hotreset_test] | 0x0:98:00.00 | DEBUG | PCIe Layer Checking.
2026-02-25 17:00:20,268 | [hotreset_test] | 0x0:98:00.00 | INFO | PCIe Link Ready Time(from reset to DL_Active=1b) is 112.35 ms.
2026-02-25 17:00:20,269 | [hotreset_test] | 0x0:98:00.00 | INFO | Will wait 100ms after Data Link Layer Link is active.
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | INFO | Wait 0.3 second for PCIe Checking delay.
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | INFO | PCIe Configuration Ready Time is less than 513.03 ms.(from reset to the First Successful CfgRd)
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | DEBUG | Endpoint Target Link Speed is 6.
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | DEBUG | Root Complex Target Link Speed is 5.
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | INFO | The device is able to reach Gen5x4.(EP Link Capability is Gen6x4; RC link Capability is Gen5x16)
2026-02-25 17:00:20,669 | [hotreset_test] | 0x0:98:00.00 | ERROR | Endpoint Correctable Error is 0x1.
2026-02-25 17:00:20,670 | [hotreset_test] | WARNING | [0x0:98: 0.0]PCIe Failures detected.
2026-02-25 17:00:20,670 | [hotreset_test] | 0x0:98:00.00 | INFO | Software did Config Write(0x0)=0x88888888 to trigger the failure.
2026-02-25 17:00:20,670 | [hotreset_test] | 0x0:98:00.00 | ERROR | PCIe Link Error is detected.
2026-02-25 17:00:20,670 | [hotreset_test] | BROADCAST | INFO | Save dmesg "DMESG_cycle_307_2026-02-25_17-00-20.log"...
2026-02-25 17:00:20,685 | [hotreset_test] | 0x0:98:00.00 | INFO | Save PCIe Configuration Space.
2026-02-25 17:00:20,686 | [hotreset_test] | 0x0:98:00.00 | INFO | File (/root/DVT/Log/PCIeConfigSpace_cycle_307_0x0-97-1-0_February_25_2026-17_00_20_686015.log) will be created.
2026-02-25 17:00:20,695 | [hotreset_test] | 0x0:98:00.00 | INFO | File (/root/DVT/Log/PCIeConfigSpace_cycle_307_0x0-98-0-0_February_25_2026-17_00_20_695748.log) will be created.
2026-02-25 17:00:20,705 | [hotreset_test] | 0x0:98:00.00 | DEBUG | SKIP NVMe Level Check, since no NVMe Error bit was set.
2026-02-25 17:00:20,706 | [hotreset_test] | WARNING | The test was paused because of ERROR.

Click Enter to continue...
2026-02-25 17:02:20,029 | [hotreset_test] | BROADCAST | INFO | Wait 1.000 sec before reach next cycle.
2026-02-25 17:02:21,030 | [hotreset_test] | BROADCAST | WARNING | The test has been terminated.
2026-02-25 17:02:21,030 | [hotreset_test] | BROADCAST | INFO | The test is done.
         Test Name  Number of Drives                                           DUT Info  ... End of The Test Start Timestamp Test Duration
0  [hotreset_test]                 1  [{'EP Address': '0x0:98:00.00', 'RC Address': ...  ...             TBD    1.772056e+09           TBD

[1 rows x 14 columns]
2026-02-25 17:02:21,158 | [hotreset_test] | INFO    | Generate HTML Test Report.
2026-02-25 17:02:21,160 | [hotreset_test] | 0x0:98:00.00 | ERROR | count_ep_correctable_error_failure: 1
2026-02-25 17:02:21,165 | [hotreset_test] | ERROR   | DB ERROR: upload_to_db diabled
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | ============================================================
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | FAILURE SUMMARY
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | Test: [hotreset_test]  |  Total Cycles Run: 306
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | ============================================================
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | DUT: 0:98:00.00  |  S/N: N/A
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO |   PCIe Failures: 1
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | ERROR |     [PCIe] Endpoint Correctable Error is 0x1.
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO |   NVMe Failures: 0
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | ============================================================
2026-02-25 17:02:21,165 | [hotreset_test] | BROADCAST | INFO | Save DVT Logs into a Zip file.
2026-02-25 17:02:21,175 | [hotreset_test] | BROADCAST | DEBUG | 10 Files Transfer from "/root/DVT/Log" to "/root/DVT/Backup/hotreset_test_2026-02-25_17-02-21.zip".
2026-02-25 17:02:21,175 | [hotreset_test] | BROADCAST | DEBUG | DVT Test Running Status Has Been Changed To "DONE"...
2026-02-25 17:02:21,176 | [hotreset_test] | DEBUG   | Cannot Find File '/root/DVT/Config/wd_dvt.cfg'.
2026-02-25 17:02:21,176 | [hotreset_test] | DEBUG   | Delete 'testname.ini' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'DVT.temp' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'status.json' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'host.json' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'dvt_running_status.json' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'Device_0000_98_00_0_INFO.json' FIle.
2026-02-25 17:02:21,176 | unknown | DEBUG   | Delete 'log_path_book.json' FIle.
2026-02-25 17:02:21,176 | unknown | INFO    | Test[0][hotreset_test] has been done.
[root@sjac DVT]#

Here is the script I have to read the errors ( you can find the script in /root/hoang.latest/pcie)

[root@sjac pcie]# source ./check_aer_read_only.sh 98:00.0 2
0000:98:00.0=/sys/bus/pci/devices/0000:98:00.0
rc_bdf=0000:97:01.0 ep_bdf=0000:98:00.0
Running with initial state of gen5x4
Clearing initial EP AER error: aer=1
Clearing initial EP lane error: lane_error=0x4
ERROR: EP aer=1 iteration=1
ERROR: EP lane_error=0x4 iteration=1
ERROR: EP aer=1 iteration=2
ERROR: EP lane_error=0x4 iteration=2
Total EP AER error count: 2
Total EP UAER error count: 0
Total RC AER error count: 0
Total RC UAER error count: 0
Total EP lane error count: 2
Total EP link down error count: 0


