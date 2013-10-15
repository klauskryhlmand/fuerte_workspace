; Auto-generated. Do not edit!


(cl:in-package FroboMsgs-msg)


;//! \htmlinclude pwm_o.msg.html

(cl:defclass <pwm_o> (roslisp-msg-protocol:ros-message)
  ((speed_left
    :reader speed_left
    :initarg :speed_left
    :type cl:integer
    :initform 0)
   (speed_right
    :reader speed_right
    :initarg :speed_right
    :type cl:integer
    :initform 0)
   (direction_left
    :reader direction_left
    :initarg :direction_left
    :type cl:integer
    :initform 0)
   (direction_right
    :reader direction_right
    :initarg :direction_right
    :type cl:integer
    :initform 0)
   (enable_left
    :reader enable_left
    :initarg :enable_left
    :type cl:integer
    :initform 0)
   (enable_right
    :reader enable_right
    :initarg :enable_right
    :type cl:integer
    :initform 0)
   (direction
    :reader direction
    :initarg :direction
    :type cl:integer
    :initform 0))
)

(cl:defclass pwm_o (<pwm_o>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <pwm_o>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'pwm_o)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name FroboMsgs-msg:<pwm_o> is deprecated: use FroboMsgs-msg:pwm_o instead.")))

(cl:ensure-generic-function 'speed_left-val :lambda-list '(m))
(cl:defmethod speed_left-val ((m <pwm_o>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:speed_left-val is deprecated.  Use FroboMsgs-msg:speed_left instead.")
  (speed_left m))

(cl:ensure-generic-function 'speed_right-val :lambda-list '(m))
(cl:defmethod speed_right-val ((m <pwm_o>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:speed_right-val is deprecated.  Use FroboMsgs-msg:speed_right instead.")
  (speed_right m))

(cl:ensure-generic-function 'direction_left-val :lambda-list '(m))
(cl:defmethod direction_left-val ((m <pwm_o>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:direction_left-val is deprecated.  Use FroboMsgs-msg:direction_left instead.")
  (direction_left m))

(cl:ensure-generic-function 'direction_right-val :lambda-list '(m))
(cl:defmethod direction_right-val ((m <pwm_o>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:direction_right-val is deprecated.  Use FroboMsgs-msg:direction_right instead.")
  (direction_right m))

(cl:ensure-generic-function 'enable_left-val :lambda-list '(m))
(cl:defmethod enable_left-val ((m <pwm_o>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:enable_left-val is deprecated.  Use FroboMsgs-msg:enable_left instead.")
  (enable_left m))

(cl:ensure-generic-function 'enable_right-val :lambda-list '(m))
(cl:defmethod enable_right-val ((m <pwm_o>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:enable_right-val is deprecated.  Use FroboMsgs-msg:enable_right instead.")
  (enable_right m))

(cl:ensure-generic-function 'direction-val :lambda-list '(m))
(cl:defmethod direction-val ((m <pwm_o>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader FroboMsgs-msg:direction-val is deprecated.  Use FroboMsgs-msg:direction instead.")
  (direction m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <pwm_o>) ostream)
  "Serializes a message object of type '<pwm_o>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'speed_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'speed_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'speed_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'speed_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'speed_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'speed_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'speed_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'speed_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'direction_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'direction_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'direction_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'direction_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'direction_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'direction_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'direction_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'direction_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'enable_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'enable_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'enable_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'enable_left)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'enable_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'enable_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'enable_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'enable_right)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'direction)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'direction)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'direction)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'direction)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <pwm_o>) istream)
  "Deserializes a message object of type '<pwm_o>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'speed_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'speed_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'speed_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'speed_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'speed_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'speed_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'speed_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'speed_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'direction_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'direction_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'direction_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'direction_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'direction_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'direction_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'direction_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'direction_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'enable_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'enable_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'enable_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'enable_left)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'enable_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'enable_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'enable_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'enable_right)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'direction)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'direction)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'direction)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'direction)) (cl:read-byte istream))
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
  "1522611c8025ddba76f26a25869646d3")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'pwm_o)))
  "Returns md5sum for a message object of type 'pwm_o"
  "1522611c8025ddba76f26a25869646d3")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<pwm_o>)))
  "Returns full string definition for message of type '<pwm_o>"
  (cl:format cl:nil "uint32 speed_left~%uint32 speed_right~%uint32 direction_left~%uint32 direction_right~%uint32 enable_left~%uint32 enable_right~%uint32 direction~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'pwm_o)))
  "Returns full string definition for message of type 'pwm_o"
  (cl:format cl:nil "uint32 speed_left~%uint32 speed_right~%uint32 direction_left~%uint32 direction_right~%uint32 enable_left~%uint32 enable_right~%uint32 direction~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <pwm_o>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <pwm_o>))
  "Converts a ROS message object to a list"
  (cl:list 'pwm_o
    (cl:cons ':speed_left (speed_left msg))
    (cl:cons ':speed_right (speed_right msg))
    (cl:cons ':direction_left (direction_left msg))
    (cl:cons ':direction_right (direction_right msg))
    (cl:cons ':enable_left (enable_left msg))
    (cl:cons ':enable_right (enable_right msg))
    (cl:cons ':direction (direction msg))
))
