//
// Copyright 2022 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef GRPC_CORE_EXT_FILTERS_CLIENT_CHANNEL_LB_POLICY_OOB_BACKEND_METRIC_H
#define GRPC_CORE_EXT_FILTERS_CLIENT_CHANNEL_LB_POLICY_OOB_BACKEND_METRIC_H

#include <grpc/support/port_platform.h>

#include <memory>

#include "src/core/ext/filters/client_channel/lb_policy.h"
#include "src/core/ext/filters/client_channel/subchannel_interface.h"

namespace grpc_core {

// Interface for LB policies to access out-of-band backend metric data
// from a subchannel.  The data is reported from via an ORCA stream
// established on the subchannel whenever an LB policy registers a
// watcher.
//
// To use this, an LB policy will implement its own subclass of
// OobBackendMetricWatcher, which will receive backend metric data as it
// is sent by the server.  It will then register that watcher with the
// subchannel like this:
//   subchannel->AddDataWatcher(
//       MakeOobBackendMetricWatcher(
//           absl::make_unique<MyOobBackendMetricWatcherSubclass>(...)));

class OobBackendMetricWatcher {
 public:
  virtual ~OobBackendMetricWatcher() = default;

  virtual void OnBackendMetricReport(
      const LoadBalancingPolicy::BackendMetricAccessor::BackendMetricData&
          backend_metric_data) = 0;
};

std::unique_ptr<SubchannelInterface::DataWatcherInterface>
MakeOobBackendMetricWatcher(Duration report_interval,
                            std::unique_ptr<OobBackendMetricWatcher> watcher);

}  // namespace grpc_core

#endif  // GRPC_CORE_EXT_FILTERS_CLIENT_CHANNEL_LB_POLICY_OOB_BACKEND_METRIC_H
