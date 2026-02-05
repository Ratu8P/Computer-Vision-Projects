# Computer-Vision-Projects
import requests
from requests.auth import HTTPBasicAuth

HOST = "192.168.1.100"
USER = "admin"
PASS = "admin"
TIMEOUT = 5

def nb_get(query: str) -> str:
    # query examples: "$A5" or "$A3+1+0"
    url = f"http://{HOST}/cmd.cgi?{query}"
    r = requests.get(url, auth=HTTPBasicAuth(USER, PASS), timeout=TIMEOUT)
    r.raise_for_status()
    return r.text.strip()

def status_bits() -> str:
    # returns like "11" from "11,"
    raw = nb_get("$A5")
    bits = raw.split(",")[0]
    return bits

def outlet_states() -> list[bool]:
    bits = status_bits()     # e.g. "11"
    # right-most bit is usually outlet 1
    return [c == "1" for c in bits[::-1]]

def set_outlet(outlet: int, on: bool) -> str:
    # $A3 outlet state(0/1)
    return nb_get(f"$A3+{outlet}+{1 if on else 0}")

def reboot_outlet(outlet: int) -> str:
    # $A4 outlet
    return nb_get(f"$A4+{outlet}")

if __name__ == "__main__":
    print("bits:", status_bits())
    print("states:", outlet_states())  # [True, True] means outlet1,on outlet2,on

    print("turn outlet1 off:", set_outlet(1, False))
    print("turn outlet1 on:", set_outlet(1, True))
    print("reboot outlet2:", reboot_outlet(2))
