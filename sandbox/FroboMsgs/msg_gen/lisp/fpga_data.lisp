; Auto-generated. Do not edit!


(cl:in-package FroboMsgs-msg)


;//! \htmlinclude fpga_data.msg.html

(cl:defclass <fpga_data> (roslisp-msg-protocol:ros-message)
  ((encoder_l
    :reader encoder_l
    :initarg :encoder_l
    :type cl:integer
    :initform 0)
   (encoder_r
    :reader encoder_r
    :initarg :encoder_r
    :type cl:integer
    :initform 0)
   (pwm_value
    :reader pwm_value
    :initarg :pwm_value
    :type cl:integer
    :initform 0))
)

(cl:defclass fpga_data (<fpga_data>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <fpga_data>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'fpga_data)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name FroboMsgs-msg:<fpga_data> is deprecated: use FroboMsgs-msg:fpga_data instead.")))

(cl:ensure-generic-function 'encoder_l-val :lambda-list '(m))
(cl:defmethod encoder_l-val ((m <fpga_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:encoder_l-val is deprecated.  Use FroboMsgs-msg:encoder_l instead.")
  (encoder_l m))

(cl:ensure-generic-function 'encoder_r-val :lambda-list '(m))
(cl:defmethod encoder_r-val ((m <fpga_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:encoder_r-val is deprecated.  Use FroboMsgs-msg:encoder_r instead.")
  (encoder_r m))

(cl:ensure-generic-function 'pwm_value-val :lambda-list '(m))
(cl:defmethod pwm_value-val ((m <fpga_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:pwm_value-val is deprecated.  Use FroboMsgs-msg:pwm_value instead.")
  (pwm_value m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <fpga_data>) ostream)
  "Serializes a message object of type '<fpga_data>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'encoder_l)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'encoder_l)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'encoder_l)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'encoder_l)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'encoder_r)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'encoder_r)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'encoder_r)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'encoder_r)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'pwm_value)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'pwm_value)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'pwm_value)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'pwm_value)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <fpga_data>) istream)
  "Deserializes a message object of type '<fpga_data>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'encoder_l)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'encoder_l)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'encoder_l)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'encoder_l)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'encoder_r)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'encoder_r)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'encoder_r)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'encoder_r)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'pwm_value)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'pwm_value)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'pwm_value)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'pwm_value)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<fpga_data>)))
  "Returns string type for a message object of type '<fpga_data>"
  "FroboMsgs/fpga_data")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'fpga_data)))
  "Returns string type for a message object of type 'fpga_data"
  "FroboMsgs/fpga_data")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<fpga_data>)))
  "Returns md5sum for a message object of type '<fpga_data>"
  "378edc8b8a9c23602b7d54ff55da0269")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'fpga_data)))
  "Returns md5sum for a message object of type 'fpga_data"
  "378edc8b8a9c23602b7d54ff55da0269")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<fpga_data>)))
  "Returns full string definition for message of type '<fpga_data>"
  (cl:format cl:nil "uint32 encoder_l~%uint32 encoder_r~%uint32 pwm_value~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'fpga_data)))
  "Returns full string definition for message of type 'fpga_data"
  (cl:format cl:nil "uint32 encoder_l~%uint32 encoder_r~%uint32 pwm_value~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <fpga_data>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <fpga_data>))
  "Converts a ROS message object to a list"
  (cl:list 'fpga_data
    (cl:cons ':encoder_l (encoder_l msg))
    (cl:cons ':encoder_r (encoder_r msg))
    (cl:cons ':pwm_value (pwm_value msg))
))
