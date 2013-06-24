FILE(REMOVE_RECURSE
  "../src/FroboMsgs/msg"
  "../msg_gen"
  "../msg_gen"
  "CMakeFiles/ROSBUILD_genmsg_lisp"
  "../msg_gen/lisp/pwm_o.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_pwm_o.lisp"
  "../msg_gen/lisp/fpga_data.lisp"
  "../msg_gen/lisp/_package.lisp"
  "../msg_gen/lisp/_package_fpga_data.lisp"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_lisp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
