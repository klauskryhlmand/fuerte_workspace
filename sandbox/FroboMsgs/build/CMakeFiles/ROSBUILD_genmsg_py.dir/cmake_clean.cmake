FILE(REMOVE_RECURSE
  "../src/FroboMsgs/msg"
  "../msg_gen"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/FroboMsgs/msg/__init__.py"
  "../src/FroboMsgs/msg/_pwm_o.py"
  "../src/FroboMsgs/msg/_fpga_data.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
