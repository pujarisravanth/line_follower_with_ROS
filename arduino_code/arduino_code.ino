/**
   This code for basic line following with straight line, curves
   and L junctions. for T junctions and unknown paths code is 
   in the nwxt file.
**/

// defining libraries
#include <ros.h>
#include <std_msgs/Int32.h>

//initialising the node with object n
ros::NodeHandle n;

//writing the publisher node
std_msgs::Int32 lnfr;
ros::Publisher pub("bot", &lnfr);

//defing pins connected to IC L293D
#define rep 11        //right enable pin
#define lep 10        //left enable pin
#define lm1 7         //left motor 1
#define lm2 8         //left motor 2
#define rm1 5         //right motor 1
#define rm2 6         //right motor 2

//defing pins for 5 array IR sensors.
#define sl2 2         //external left sensor
#define sl1 3         //internal left sensor
#define sm  12        //middle sensor
#define sr1 9         //internal right sensor
#define sr2 4         //external right sensor

//function for moving forward
void move_forward(){
  analogWrite(lep,70);
  analogWrite(rep,70);
  digitalWrite(rm1,HIGH);
  digitalWrite(lm1,HIGH);
  digitalWrite(lm2,LOW);
  digitalWrite(rm2,LOW);
  delay(1);
}

//function for sway right
void sway_right(){
  analogWrite(rep,80);
  analogWrite(lep,60);
  digitalWrite(rm1,HIGH);
  digitalWrite(lm1,HIGH);
  digitalWrite(lm2,LOW);
  digitalWrite(rm2,LOW);
  delay(5);
}

//function to sway left
void sway_left(){
  analogWrite(rep,60);
  analogWrite(lep,80);
  digitalWrite(rm1,HIGH);
  digitalWrite(lm1,HIGH);
  digitalWrite(lm2,LOW);
  digitalWrite(rm2,LOW);
  delay(5);
}

//function to turn right
void turn_right(){
  analogWrite(lep,200);
  analogWrite(rep,200);
  digitalWrite(rm1,LOW);
  digitalWrite(lm1,HIGH);
  digitalWrite(lm2,LOW);
  digitalWrite(rm2,HIGH);
  delay(20);
}

//function to turn left
void turn_left(){
  analogWrite(lep,200);
  analogWrite(rep,200);
  digitalWrite(rm1,HIGH);
  digitalWrite(lm1,LOW);
  digitalWrite(lm2,HIGH);
  digitalWrite(rm2,LOW);
  delay(20);
}

//callback function, which was called by subscriber node
void callback(const std_msgs::Int32 &lnfr){
  if(lnfr.data == 2) move_forward();
  else if(lnfr.data == 3) sway_right();
  else if(lnfr.data == 4) sway_left();
  else if(lnfr.data == 5) turn_right();
  else if(lnfr.data == 6) turn_left();
}

//writing subscriber node
ros::Subscriber <std_msgs::Int32> sub("bot", &callback);

void setup(){
  pinMode (lep, OUTPUT);
  pinMode (rep, OUTPUT);
  pinMode (lm1, OUTPUT);
  pinMode (lm2, OUTPUT);
  pinMode (rm1, OUTPUT);
  pinMode (rm2, OUTPUT);
  pinMode (sl2, INPUT);
  pinMode (sl1, INPUT);
  pinMode (sm, INPUT);
  pinMode (sr1, INPUT);
  pinMode (sr2, INPUT);
  
  //initialising ROS nodes and publishing messages
  n.initNode();        
  n.advertise(pub);
}

void loop(){
  //condition for moving straight
  if(digitalRead(l1_sen) == HIGH && digitalRead(r1_sen) == HIGH){
    lnfr.data = 2;
  }
  
  //condition for turning left
  else if(digitalRead(l1_sen) == LOW && digitalRead(l2_sen) == LOW){
    lnfr.data = 6;
  }
  
  // condition for turn right
  else if(digitalRead(r1_sen) == LOW && digitalRead(r2_sen) == LOW){
    lnfr.data = 5;
  }
  
  //condition for sway_left
  else if((digitalRead(l1_sen) == LOW && digitalRead(r1_sen) == HIGH) || 
    (digitalRead(l2_sen) == LOW && digitalRead(l1_sen) == HIGH)){
    lnfr.data = 4;
  }
  
  //condition for sway_right
  else if((digitalRead(r1_sen) == LOW && digitalRead(l1_sen) == HIGH) || 
    (digitalRead(r2_sen) == LOW && digitalRead(r1_sen) == HIGH)){
    lnfr.data = 3;
  }
  
  //publishing the information we got from sensors
  pub.publish(&lnfr);
  n.spinOnce();
  
}
