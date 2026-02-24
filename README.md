# Computer-Vision-Projects

  def print_failure_summary(self):
      separator = "=" * 60
      has_any_failure = any(
          dut.pcie_failure.failure_count != 0 or dut.nvme_failure.failure_count != 0
          for dut in self.device_array
      )
      self.broadcast_logger.info(separator)
      self.broadcast_logger.info("FAILURE SUMMARY")
      self.broadcast_logger.info(
          f"Test: {self.TestName}  |  Total Cycles Run: {self.cycle - 1}"
      )
      self.broadcast_logger.info(separator)
      if not has_any_failure:
          self.broadcast_logger.info("No errors detected. All cycles passed.")
          self.broadcast_logger.info(separator)
          return
      for dut in self.device_array:
          pcie_count = dut.pcie_failure.failure_count
          nvme_count = dut.nvme_failure.failure_count
          if pcie_count == 0 and nvme_count == 0:
              continue
          self.broadcast_logger.info(f"DUT: {dut.dut_name}  |  S/N: {dut.serial_number}")
          self.broadcast_logger.info(f"  PCIe Failures: {pcie_count}")
          for desc in dut.pcie_failure.failure_description:
              self.broadcast_logger.error(f"    [PCIe] {desc}")
          self.broadcast_logger.info(f"  NVMe Failures: {nvme_count}")
          for desc in dut.nvme_failure.failure_description:
              self.broadcast_logger.error(f"    [NVMe] {desc}")
      self.broadcast_logger.info(separator)
