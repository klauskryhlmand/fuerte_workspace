FILE(REMOVE_RECURSE
  "../src/FroboMsgs/msg"
  "../msg_gen"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/FroboMsgs/msg/__init__.py"
  "../src/FroboMsgs/msg/_detected_objects.py"
  "../src/FroboMsgs/msg/_float_data.py"
  "../src/FroboMsgs/msg/_pwm_o.py"
  "../src/FroboMsgs/msg/_fpga_data.py"
  "../src/FroboMsgs/msg/_accelerometer.py"
  "../src/FroboMsgs/msg/_gyroscope.py"
  "../src/FroboMsgs/msg/_hilde_states.py"
  "../src/FroboMsgs/msg/_error.py"
  "../src/FroboMsgs/msg/_magnetometer.py"
  "../src/FroboMsgs/msg/_imu_razor_9dof.py"
  "../src/FroboMsgs/msg/_object_row.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
