FILE(REMOVE_RECURSE
  "../src/FroboMsgs/msg"
  "../msg_gen"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_cpp"
  "../msg_gen/cpp/include/FroboMsgs/detected_objects.h"
  "../msg_gen/cpp/include/FroboMsgs/float_data.h"
  "../msg_gen/cpp/include/FroboMsgs/pwm_o.h"
  "../msg_gen/cpp/include/FroboMsgs/fpga_data.h"
  "../msg_gen/cpp/include/FroboMsgs/accelerometer.h"
  "../msg_gen/cpp/include/FroboMsgs/gyroscope.h"
  "../msg_gen/cpp/include/FroboMsgs/hilde_states.h"
  "../msg_gen/cpp/include/FroboMsgs/error.h"
  "../msg_gen/cpp/include/FroboMsgs/magnetometer.h"
  "../msg_gen/cpp/include/FroboMsgs/imu_razor_9dof.h"
  "../msg_gen/cpp/include/FroboMsgs/object_row.h"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_cpp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
