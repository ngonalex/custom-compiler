
The Really Cool Compiler for INFLUX
=======================================

## Building

Compile subprojects by giving their paths and task names defined in the respective BUILD files to `bazel build`.

The main project, in the public directory, for example, is built by `bazel build //public:main`. If the directory and task in BUILD file have the same name, you can omit the task name.

## Testing

Test in the same way, by using `bazel test`. Make sure to include the library/binary that you are testing as a dependency in the test task.

## Running

To run a certain subproject, you can use `bazel run` with the path and name of the task.

