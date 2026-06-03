# romea_core_rtls

## 1. Overview

`romea_core_rtls` is a framework-independent C++ library for working with real-time locating systems based on range measurements between radio transceivers.

The package gathers the core RTLS concepts in a single place:

* an abstract transceiver interface, including how a transceiver is identified and which function it can play during a ranging exchange;
* ranging request, response, result and status types used to ask for distance measurements and describe their outcome;
* ranging schedulers and diagnostics;
* compact pose and twist serialization helpers for exchanging data during ranging operations;
* 2D position and pose estimation from range measurements.

The package does not provide a complete transceiver implementation. The actual ranging operation depends on the hardware architecture, firmware protocol or simulator backend, so hardware-specific and middleware-specific drivers are intentionally kept outside this library.

---

## 2. Transceiver and Ranging Model

An RTLS deployment is represented as a set of transceivers. Each transceiver has an extended unique identifier and one of three operational functions:

* initiators start ranging exchanges;
* responders answer initiator requests;
* listeners observe exchanges between initiators and responders and recover ranging information for remote processing.

Initiators measure distances to responders so that trilateration or pose estimation algorithms can be applied afterwards. In most UWB systems, the distance is obtained from time-of-flight information through two-way ranging exchanges implemented by the transceiver firmware or low-level driver.

| Concept | API | Role |
| ------- | --- | ---- |
| Transceiver interface | `RTLSTransceiver` | Abstract base class used to implement hardware-specific or simulator-specific ranging. |
| Transceiver identity | `RTLSTransceiver::EUID` | Stores the PAN identifier and local transceiver identifier. |
| Transceiver function | `RTLSTransceiver::Function` | Describes whether a transceiver acts as `INITIATOR`, `RESPONDER`, `LISTENER` or `NONE`. |
| Ranging request | `RTLSRangingRequest` | Carries the responder identifier, timeout and optional payload sent during a ranging exchange. |
| Ranging response | `RTLSRangingResponse` | Carries the measured range result and optional payload returned by the responder. |
| Ranging result | `RTLSRangingResult` | Stores range, exchange duration and received power indicators. |

`RTLSTransceiver` is an abstract base class. Hardware-specific or simulator-specific transceivers implement the `ranging()` method and translate `RTLSRangingRequest` objects into the communication mechanism available on the device.

The class does not impose the radio protocol itself. It only defines the common C++ interface used by the schedulers, diagnostics and estimation code.

---

## 3. Scheduling

RTLS localisation usually requires several distance measurements between pairs of transceivers. A scheduler is therefore needed to decide which initiator/responder pair should be ranged at each polling step.

The package provides two scheduling strategies. The first one visits every possible pair with a round-robin policy. The second one uses a current position estimate to keep only nearby responders, which avoids spending time on transceivers that are likely to be out of range.

Schedulers do not perform the low-level exchange themselves: they call a user-provided callback with the selected indexes. The callback receives indexes rather than transceiver objects so that applications can keep ownership of their own containers and communication resources.

| Scheduler | Behaviour |
| --------- | --------- |
| `RTLSRoundRobinScheduler` | Cycles through all initiator/responder pairs at a fixed polling rate. |
| `RTLSPositionAwareScheduler` | Extends round-robin scheduling by selecting responders close to the current robot position. |
| `RTLSNearbyTransceiverFinder` | Selects transceiver indexes inside a search radius. |

Ranging feedback is passed back to the scheduler with `feedback()`. The scheduler forwards it to `RTLSTransceiversDiagnostics`, then `get_report()` returns the corresponding diagnostic report.

---

## 4. Serialization

During ranging operations, transceivers can also exchange small payloads. The package provides byte-level helpers for compact 2D pose and twist data.

| Data | Serialization helpers |
| ---- | --------------------- |
| Cartesian coordinates | `serialize_cartesian_coordinate()`, `deserialize_cartesian_coordinate()` |
| 2D positions | `serialize_position2d()`, `deserialize_position2d()` |
| 2D position covariance | `serialize_position2d_covariance()`, `deserialize_position2d_covariance()` |
| Orientation | `serialize_orientation()`, `deserialize_orientation()` |
| Orientation variance | `serialize_orientation_variance()`, `deserialize_orientation_variance()` |
| 2D pose | `serialize_pose2d()`, `deserialize_pose2d()` |
| Linear speeds | `serialize_linear_speeds()`, `deserialize_linear_speeds()` |
| Linear speed covariance | `serialize_linear_speeds_covariance()`, `deserialize_linear_speeds_covariance()` |
| Angular speed | `serialize_angular_speed()`, `deserialize_angular_speed()` |
| Angular speed variance | `serialize_angular_speed_variance()`, `deserialize_angular_speed_variance()` |
| 2D twist | `serialize_twist2D()`, `deserialize_twist2D()` |

The covariance serialization helpers expect covariance-like blocks with non-negative variances and symmetric terms.

---

## 5. 2D Estimation

Range measurements can be used for trilateration. Depending on the number of available transceivers and their spatial configuration, the package can estimate either the 2D position of an object or its 2D pose.

These estimators accept optional ranges, so missing or rejected measurements can be represented with `std::nullopt` while keeping the same responder ordering. A range can be missing because two transceivers are too far apart, because the signal is blocked, or because the received signal looks unreliable. `RTLSRangingStatusEvaluator` supports this filtering step by rejecting empty results, ranges outside configured bounds and measurements where the total received power differs too much from the first-path received power, a common indicator used to detect non-line-of-sight or multipath conditions in UWB systems.

| Estimator | Input | Output |
| --------- | ----- | ------ |
| `SimpleTrilateration2D` | 2D responder positions and ranges. | Direct 2D position estimate from range geometry. |
| `RTLSPosition2DEstimator` | Known responder positions and optional ranges. | 2D position estimate using nonlinear least squares. |
| `RTLSPose2DEstimator` | Target transceiver positions, responder positions and optional range arrays. | 2D pose estimate using nonlinear least squares. |

The nonlinear estimators derive from the least-squares tools provided by `romea_core_common`.

---

## 6. Ranging Status and Diagnostics

The package provides small helpers to qualify range measurements and monitor the reliability of transceivers over time.

| Helper | Role |
| ------ | ---- |
| `isEmpty()` | Detects empty ranging results. |
| `RTLSRangingStatusEvaluator` | Classifies a result as `FAILED`, `UNAVAILABLE` or `AVAILABLE` from range bounds and received power levels. |
| `RTLSTransceiversDiagnostics` | Maintains initiator and responder reliability diagnostics from ranging feedback. |

These classes are used by the schedulers, but they can also be used directly by applications that manage their own ranging loop.

---

## 7. Minimal Usage

The following example shows the intended separation between the core RTLS interface, the scheduling logic and the estimation step. A concrete transceiver derives from `RTLSTransceiver` and implements the hardware-specific `ranging()` method. The scheduler chooses which pair must be ranged, while the application stores the resulting ranges and feeds them to an estimator.

In this example, `responders[i]`, `responder_positions[i]` and `ranges[i]` refer to the same physical responder.

```cpp
#include <optional>
#include <string>
#include <vector>

#include "romea_core_common/containers/Eigen/VectorOfEigenVector.hpp"
#include "romea_core_rtls/scheduling/round_robin_scheduler.hpp"
#include "romea_core_rtls/transceiver.hpp"
#include "romea_core_rtls/trilateration/position2D_estimator.hpp"

class MyTransceiver : public romea::core::RTLSTransceiver {
 public:
  explicit MyTransceiver(const EUID& euid, const Function& function)
      : RTLSTransceiver(euid, function) {}

  romea::core::RTLSRangingResponse ranging(
      const romea::core::RTLSRangingRequest& request) override {
    romea::core::RTLSRangingResponse response;

    // Placeholder for a hardware-specific or simulator-specific ranging
    // implementation.
    response.ranging_result.range = perform_two_way_ranging(request);
    return response;
  }
};

std::vector<MyTransceiver> initiators;
initiators.emplace_back(romea::core::RTLSTransceiver::EUID{0x1234, 0},
                        romea::core::RTLSTransceiver::Function::INITIATOR);

std::vector<MyTransceiver> responders;
responders.emplace_back(romea::core::RTLSTransceiver::EUID{0x1234, 1},
                        romea::core::RTLSTransceiver::Function::RESPONDER);
responders.emplace_back(romea::core::RTLSTransceiver::EUID{0x1234, 2},
                        romea::core::RTLSTransceiver::Function::RESPONDER);
responders.emplace_back(romea::core::RTLSTransceiver::EUID{0x1234, 3},
                        romea::core::RTLSTransceiver::Function::RESPONDER);

romea::core::VectorOfEigenVector3d responder_positions;
responder_positions.emplace_back(0.0, 0.0, 1.0);
responder_positions.emplace_back(5.0, 0.0, 1.0);
responder_positions.emplace_back(0.0, 5.0, 1.0);

romea::core::RTLSPosition2DEstimator::RangeVector ranges(responders.size());

auto ranging_callback =
    [&](const size_t& initiator_index, const size_t& responder_index,
        const romea::core::Duration& timeout) {
      romea::core::RTLSRangingRequest request;
      request.responder_id = responders[responder_index].get_euid().id;
      request.timeout = romea::core::durationToSecond(timeout);

      auto response = initiators[initiator_index].ranging(request);
      ranges[responder_index] = response.ranging_result.range;
    };

// The scheduler may run asynchronously. In a real application, protect shared
// range storage if it is accessed from another thread.
romea::core::RTLSRoundRobinScheduler scheduler(
    20.0, {"initiator0"}, {"responder0", "responder1", "responder2"},
    ranging_callback);

// scheduler.start();
// ...
// scheduler.stop();

romea::core::RTLSPosition2DEstimator estimator(responder_positions, 0.001);

if (estimator.init(ranges) && estimator.estimate(20, 0.02)) {
  Eigen::VectorXd position = estimator.getEstimate();
}
```

---

## 8. Related Packages

| Package | Role |
| ------- | ---- |
| `romea_core_common` | Provides common time, diagnostics, monitoring, geometry, containers and nonlinear least-squares utilities used by this package. |

---

## License

This project is released under the Apache License 2.0. See the `LICENSE` file for details.

## Authors

This library was developed by **Jean Laneurit** in the context of several research projects carried out at INRAE.
