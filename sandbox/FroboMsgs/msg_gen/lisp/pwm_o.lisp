; Auto-generated. Do not edit!


(cl:in-package FroboMsgs-msg)


;//! \htmlinclude pwm_o.msg.html

(cl:defclass <pwm_o> (roslisp-msg-protocol:ros-message)
  ((pwm_speed
    :reader pwm_speed
    :initarg :pwm_speed
    :type cl:integer
    :initform 0))
)

(cl:defclass pwm_o (<pwm_o>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <pwm_o>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'pwm_o)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name FroboMsgs-msg:<pwm_o> is deprecated: use FroboMsgs-msg:pwm_o instead.")))

(cl:ensure-generic-function 'pwm_speed-val :lambda-list '(m))
(cl:defmethod pwm_speed-val ((m <pwm_o>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:pwm_speed-val is deprecated.  Use FroboMsgs-msg:pwm_speed instead.")
  (pwm_speed m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <pwm_o>) ostream)
  "Serializes a message object of type '<pwm_o>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'pwm_speed)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'pwm_speed)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'pwm_speed)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'pwm_speed)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <pwm_o>) istream)
  "Deserializes a message object of type '<pwm_o>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'pwm_speed)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'pwm_speed)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'pwm_speed)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'pwm_speed)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<pwm_o>)))
  "Returns string type for a message object of type '<pwm_o>"
  "FroboMsgs/pwm_o")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'pwm_o)))
  "Returns string type for a message object of type 'pwm_o"
  "FroboMsgs/pwm_o")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<pwm_o>)))
  "Returns md5sum for a message object of type '<pwm_o>"
  "a7a55754d53c25a62a32c681d73ec18f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'pwm_o)))
  "Returns md5sum for a message object of type 'pwm_o"
  "a7a55754d53c25a62a32c681d73ec18f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<pwm_o>)))
  "Returns full string definition for message of type '<pwm_o>"
  (cl:format cl:nil "uint32 pwm_speed~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'pwm_o)))
  "Returns full string definition for message of type 'pwm_o"
  (cl:format cl:nil "uint32 pwm_speed~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <pwm_o>))
  (cl:+ 0
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <pwm_o>))
  "Converts a ROS message object to a list"
  (cl:list 'pwm_o
    (cl:cons ':pwm_speed (pwm_speed msg))
))
