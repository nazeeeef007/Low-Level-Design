# Elevator system

A thread-safe C++17 implementation of the supplied elevator-system design.

## Design

- `ElevatorSystem` dispatches hall calls through an interchangeable
  `ElevatorSelectionStrategy`.
- `NearestElevatorStrategy` prioritizes elevators already travelling in the
  requested direction, then proximity and queue length.
- Each `Elevator` owns one worker thread and ordered up/down stop sets. Duplicate
  stops are coalesced automatically.
- `IdleState`, `MovingUpState`, and `MovingDownState` implement movement state.
- `ElevatorObserver` receives immutable snapshots; `Display` is a console observer.
- Capacity changes are synchronized and over-capacity boarding is rejected.

## Build and run

```sh
cmake -S elevator -B elevator/build
cmake --build elevator/build
ctest --test-dir elevator/build --output-on-failure
./elevator/build/elevator_demo
```

On a multi-config generator, add `--config Debug` to the build and test commands.
