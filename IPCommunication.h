  #ifndef IP_COMMUNICATION_H
#define IP_COMMUNICATION_H

class IPCommunication
{

public:
  static bool setFlag(char s);

  static bool recieveData();

  static void sendData();
  static int getNumber();
  static int getNo1();
  static int getNo2();
  static int getNo3();
  static int getNo4();
  static int getNo5();
  static int getNo6();
  static int getNo7();
  static int getNo8();
  static int getBlackBlock();
  static int getColorSircle();
  // static int getBlock();
  static void decode();
  static int returnmessage();
  //static int returnsize();
  static bool isFinished();
  static bool isCompleted();
  static bool getCourceInfo_sim();
  static int getBlockSpot_sim(int colorblock);
  static int getCardNumber_sim();
  static int getBlockNumber_sim();

private:
};

#endif