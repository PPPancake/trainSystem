#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>

extern int activeUserId; //当前登录用户id
extern int activeMainIndex; //当前首页widget界面Index
extern int activeStackIndex; //当前车票信息界面index
extern int activeOrder; //用户选中的要操作的订单
class order//订单类型
{
   public:
      int user_id; //用户id
      QString idcard; //用户身份证
      QString username; //用户名
      QString tele; //用户联系方式
      int train_id; //列车id
      QString train_name; //列车车次
      QString carriage; //车厢id
      QString seat_type; //座位类型
      int seat_id; //座位id
      QString seat_location; //座位具体位置
      QString start_time; //发车时间
      int start_station_id; //起点站id
      QString start_station_name; //起点站
      QString end_time; //到达时间
      int end_station_id; //到达站id
      QString end_station_name; //到达站
      QString date; //日期
      QString create_date; //购票时间
      int miles; //全程距离
      QString num; //票号
};

#endif // GLOBAL_H
