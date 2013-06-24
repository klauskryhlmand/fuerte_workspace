
(cl:in-package :asdf)

(defsystem "FroboMsgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "pwm_o" :depends-on ("_package_pwm_o"))
    (:file "_package_pwm_o" :depends-on ("_package"))
    (:file "fpga_data" :depends-on ("_package_fpga_data"))
    (:file "_package_fpga_data" :depends-on ("_package"))
  ))