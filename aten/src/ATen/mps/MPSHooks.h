//  Copyright © 2022 Apple Inc.

#pragma once

#include <ATen/detail/MPSHooksInterface.h>
#include <ATen/Generator.h>
#include <ATen/mps/MPSEvent.h>
#include <c10/util/Optional.h>

namespace at { namespace mps {

// The real implementation of MPSHooksInterface
struct MPSHooks : public at::MPSHooksInterface {
  MPSHooks(at::MPSHooksArgs) : m_event_pool(getMPSEventPool()) {}
  void initMPS() const override;

  // MPSDevice interface
  bool hasMPS() const override;
  bool isOnMacOS13orNewer(unsigned minor) const override;

  // MPSGeneratorImpl interface
  const Generator& getDefaultMPSGenerator() const override;

  // MPSStream interface
  void deviceSynchronize() const override;
  void commitStream() const override;
  void* getCommandBuffer() const override;
  void* getDispatchQueue() const override;

  // MPSAllocator interface
  Allocator* getMPSDeviceAllocator() const override;
  void emptyCache() const override;
  size_t getCurrentAllocatedMemory() const override;
  size_t getDriverAllocatedMemory() const override;
  void setMemoryFraction(double ratio) const override;

  // MPSProfiler interface
  void profilerStartTrace(const std::string& mode, bool waitUntilCompleted) const override;
  void profilerStopTrace() const override;

  // MPSEvent interface
  id_t acquireEvent(bool enable_timing) const override;
  void releaseEvent(id_t event_id) const override;
  void recordEvent(id_t event_id) const override;
  void waitForEvent(id_t event_id) const override;
  void synchronizeEvent(id_t event_id) const override;
  bool queryEvent(id_t event_id) const override;
  double elapsedTimeOfEvents(id_t start_event_id, id_t end_event_id) const override;

private:
  // hold a reference to MPSEventPool so it gets destroyed after MPSHooks
  std::shared_ptr<MPSEventPool> m_event_pool;
};

}} // at::mps
