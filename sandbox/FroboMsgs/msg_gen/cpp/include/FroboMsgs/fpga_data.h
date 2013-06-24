/* Auto-generated by genmsg_cpp for file /home/frobo/fuerte_workspace/sandbox/FroboMsgs/msg/fpga_data.msg */
#ifndef FROBOMSGS_MESSAGE_FPGA_DATA_H
#define FROBOMSGS_MESSAGE_FPGA_DATA_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"


namespace FroboMsgs
{
template <class ContainerAllocator>
struct fpga_data_ {
  typedef fpga_data_<ContainerAllocator> Type;

  fpga_data_()
  : encoder_l(0)
  , encoder_r(0)
  , pwm_value(0)
  {
  }

  fpga_data_(const ContainerAllocator& _alloc)
  : encoder_l(0)
  , encoder_r(0)
  , pwm_value(0)
  {
  }

  typedef uint32_t _encoder_l_type;
  uint32_t encoder_l;

  typedef uint32_t _encoder_r_type;
  uint32_t encoder_r;

  typedef uint32_t _pwm_value_type;
  uint32_t pwm_value;


  typedef boost::shared_ptr< ::FroboMsgs::fpga_data_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::FroboMsgs::fpga_data_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct fpga_data
typedef  ::FroboMsgs::fpga_data_<std::allocator<void> > fpga_data;

typedef boost::shared_ptr< ::FroboMsgs::fpga_data> fpga_dataPtr;
typedef boost::shared_ptr< ::FroboMsgs::fpga_data const> fpga_dataConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::FroboMsgs::fpga_data_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::FroboMsgs::fpga_data_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace FroboMsgs

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::FroboMsgs::fpga_data_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::FroboMsgs::fpga_data_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::FroboMsgs::fpga_data_<ContainerAllocator> > {
  static const char* value() 
  {
    return "378edc8b8a9c23602b7d54ff55da0269";
  }

  static const char* value(const  ::FroboMsgs::fpga_data_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x378edc8b8a9c2360ULL;
  static const uint64_t static_value2 = 0x2b7d54ff55da0269ULL;
};

template<class ContainerAllocator>
struct DataType< ::FroboMsgs::fpga_data_<ContainerAllocator> > {
  static const char* value() 
  {
    return "FroboMsgs/fpga_data";
  }

  static const char* value(const  ::FroboMsgs::fpga_data_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::FroboMsgs::fpga_data_<ContainerAllocator> > {
  static const char* value() 
  {
    return "uint32 encoder_l\n\
uint32 encoder_r\n\
uint32 pwm_value\n\
\n\
";
  }

  static const char* value(const  ::FroboMsgs::fpga_data_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::FroboMsgs::fpga_data_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::FroboMsgs::fpga_data_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.encoder_l);
    stream.next(m.encoder_r);
    stream.next(m.pwm_value);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct fpga_data_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::FroboMsgs::fpga_data_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::FroboMsgs::fpga_data_<ContainerAllocator> & v) 
  {
    s << indent << "encoder_l: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.encoder_l);
    s << indent << "encoder_r: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.encoder_r);
    s << indent << "pwm_value: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.pwm_value);
  }
};


} // namespace message_operations
} // namespace ros

#endif // FROBOMSGS_MESSAGE_FPGA_DATA_H

