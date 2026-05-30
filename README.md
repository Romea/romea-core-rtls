# romea_core_rtls

## Overview

`romea_core_rtls` is a C++ library for coordinating RTLS ranging exchanges, serializing compact robot state data and estimating 2D positions or poses from range measurements.

It builds on `romea_core_rtls_transceiver`, which provides the low-level transceiver types and ranging-result containers.

The package is framework-independent C++ code. Middleware-specific drivers, messages and launch files are intentionally kept outside this library.

---

## Main components

| Component | Main classes | Role |
| --------- | ------------ | ---- |
| Coordination schedulers | `RTLSSimpleCoordinatorScheduler`, `RTLSGeoreferencedCoordinatorScheduler` | Schedule ranging requests between initiators and responders. |
| Reachability selection | `RTLSReachableTransceivers` | Select responders that are reachable from a given position. |
| Diagnostics | `RTLSTransceiversDiagnostics` | Monitor initiator and responder ranging reliability. |
| Trilateration | `SimpleTrilateration2D`, `RTLSPosition2DEstimator`, `RTLSPose2DEstimator` | Estimate 2D position or pose from range measurements. |
| Serialization | `Pose2DSerialization`, `Twist2DSerialization` | Encode compact pose and twist data into byte buffers. |

---

## Coordination algorithms

The coordination layer decides which transceivers should perform a ranging exchange.

| Scheduler | Behaviour |
| --------- | --------- |
| `RTLSSimpleCoordinatorScheduler` | Cycles through initiator/responder pairs at a configured polling rate and calls a user-provided ranging request callback. |
| `RTLSGeoreferencedCoordinatorScheduler` | Extends the simple scheduler by selecting responders that are reachable from the current robot position. |

Ranging feedback is fed back to the scheduler with `feedback()`, then forwarded to `RTLSTransceiversDiagnostics` to update initiator and responder reliability.

---

## Estimation algorithms

The package provides three 2D estimation helpers:

| Estimator | Input | Output |
| --------- | ----- | ------ |
| `SimpleTrilateration2D` | Reference tag positions and ranges. | Direct 2D position estimate from range-circle intersections. |
| `RTLSPosition2DEstimator` | Reference tag positions and optional ranges. | 2D position estimate using nonlinear least squares. |
| `RTLSPose2DEstimator` | Target tag positions, reference tag positions and optional range arrays. | 2D pose estimate using nonlinear least squares. |

The nonlinear estimators inherit from the least-squares tools provided by `romea_core_common`.

---

## Serialization helpers

RTLS exchanges can carry compact robot state information in addition to range measurements.

| Serialization helpers | Data |
| --------------------- | ---- |
| `serializePose2D()` / `deserializePose2D()` | 2D position, orientation and covariance. |
| `serialize_twist2D()` / `deserialize_twist2D()` | Linear speeds, angular speed and covariance. |

Lower-level helpers are also available to encode individual coordinates, orientations, speeds and variances into byte buffers.

---

## Minimal usage

```cpp
#include <romea_core_rtls/trilateration/RTLSPosition2DEstimator.hpp>

romea::core::VectorOfEigenVector3d reference_tag_positions;

romea::core::RTLSPosition2DEstimator estimator(reference_tag_positions);

romea::core::RTLSPosition2DEstimator::RangeVector ranges;
ranges.emplace_back(2.4);
ranges.emplace_back(3.1);
ranges.emplace_back(std::nullopt);
ranges.emplace_back(4.0);

if (estimator.init(ranges)) {
  estimator.estimate(maximal_number_of_iterations, convergence_threshold);
  auto position = estimator.getEstimate();
}
```

---

## Related packages

| Package | Role |
| ------- | ---- |
| `romea_core_common` | Shared core types used for time, diagnostics, monitoring, geometry and least-squares tools. |
| `romea_core_rtls_transceiver` | RTLS transceiver identity, physical configuration, ranging result and noise models. |

---

## License

This project is released under the Apache License 2.0. See the `LICENSE` file for details.

## Authors

This library was developed by **Jean Laneurit** in the context of several research projects carried out at INRAE.
