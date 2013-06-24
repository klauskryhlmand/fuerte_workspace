FILE(REMOVE_RECURSE
  "src/beginner_tutorials/msg"
  "msg_gen"
  "msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "src/beginner_tutorials/msg/__init__.py"
  "src/beginner_tutorials/msg/_fpga_data.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
