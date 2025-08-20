
#include <iostream>
#include <string>
#include <strings.h>
#include <cstring>
#include <ctime>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include "Utils.h"

string getHiddenInput() 
{
    termios oldt, newt;
    string password;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);           // get terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ECHO);                  // disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((ch = getchar()) != '\n') {
        if (ch == 127 && !password.empty()) { // handle backspace
            password.pop_back();
            cout << "\b \b";
        } else {
            password.push_back(ch);
            cout << "*";
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // restore settings
    cout << endl;
    return password;
}


void clearScreen() 
{
    cout << "\033[2J\033[1;1H"; // clear screen and move cursor to top-left
}



void Line()
{
  for(int i=0;i<81;i++)
  cout<<"- ";
}



struct tm* Time()
{
   time_t t;
   time(&t);
   return(localtime(&t));
}




int x_coord(string s)
{

   return(110-(s.length()*2)/2);
}



void gotoxy(int x, int y) 
{
    // ANSI escape sequence: \033[row;colH
    cout << "\033[" << y << ";" << x << "H";
}



void Title(string title)
{
    gotoxy(x_coord(title),2);
    for (char ch : title)
      cout<<ch<<" ";
    gotoxy(195,2);
    cout<<asctime(Time());
    gotoxy(0,3);

    for(int i=0;i<220;i++)
        cout<<"+";
    gotoxy(0,5);
}




void Message(string MESSEGE)
{
  for(int i=0;i<MESSEGE.length();i++)
  {
    cout<<MESSEGE[i];
    this_thread::sleep_for(chrono::milliseconds(50));
  }
}




bool MobileVerifier(string MOBILE)
   {
       int count=0;
        if(MOBILE.length()==10)
        {
             if(MOBILE[0]=='6' || MOBILE[0]=='7' || MOBILE[0]=='8' || MOBILE[0]=='9')
            {
                for(int i=1;i<10;i++)
                {
                    if(MOBILE[i]<58 && MOBILE[i]>47)
                     count++;
                }
            }
        }

    if(count==9)
    return true;
    else
    return false;

   }



bool GenderVerifier(string GENDER)
{
    if(GENDER=="MALE" || GENDER=="FEMALE" || GENDER=="Male" || GENDER=="Female")
       return true;
    else
       return false;
}




bool DOBVerifier(string dob)
{   
   int flag=0;
   if(dob[2]=='/' && dob[5]=='/' && dob.length()==10)
 {
    int D,M,Y;
    D=(dob[0]-'0')*10 + (dob[1]-'0');
    M=(dob[3]-'0')*10 + (dob[4]-'0');
    Y=(dob[6]-'0')*1000 + (dob[7]-'0')*100 + (dob[8]-'0')*10 + (dob[9]-'0');
    
   if((D>=1 && D<=31) && (M>=1 && M<=12) && (Y>=1900 && Y<=2016))
    {
        if((M==1 || M==3 || M==5 || M==7 || M==8 || M==10 || M==12) && D>=1 && D<=31)
        flag=1;

        if(M==2)
        {
            if(Y%4==0 || Y%400==0)
            {
                if(D<=29 && D>=1)
                flag=1;
            }
            else if(D<=28 && D>=1)
            flag=1;
        }  
        

        if((M==4 || M==6 || M==9 || M==11) && (D>=1 && D<=30))
        flag=1;
    }
    else
    return false;
 }
 else
 return false;

    if(flag==1)
      return true;
    else
      return false; 
}




bool PasswordVerifier(string Password)
{
   int i=0,count_lower=0,count_upper=0,count_special=0,count_number=0;
    if(Password.length()>7)
    {
        while(Password[i]!='\0')
        {
            if((Password[i]>96 && Password[i]<123))
            count_lower++;

            if((Password[i]>64 && Password[i]<91))
            count_upper++;

            if((Password[i]>47 && Password[i]<58))
            count_number++;

            if((Password[i]>32 && Password[i]<48) || (Password[i]>57 && Password[i]<65) || (Password[i]>90 && Password[i]<97) && (Password[i]>122 && Password[i]<127))
            count_special++;

            i++;
        }
    }
    else
    return false;

    if(count_lower!=0 && count_number!=0 && count_special!=0 && count_upper!=0)
    return true;
    else
    return false;
}




bool EmailVerifier(string email)
{
int index1=0,index2=0,flag=0,count1=0,count2=0;
int i=0;
while(email[i]!='\0')
    {
      if((email[i]<123 && email[i]>96) || (email[i]<91 && email[i]>64) || (email[i]<58 && email[i]>47) || email[i]=='@' || email[i]=='.')
       {
          if((email[0]<123 && email[0]>96) || (email[0]<91 && email[0]>64))
          flag=1;

          if(email[i]=='@')
          {
              count1++;
          index1=email.find('@',0);
          }

          if(email[i]=='.')
          {
              count2++;
          index2=email.find('.',0);
          }
       }
       else
       {
           cout<<"This is an invalid email ID\n";
           return false;
       }
       i++;
    }

    if(flag==1 && index1!=0 && index2!=0 && index1<index2 && count1==1 && count2==1)
    return true;
    else
    return false;

}
