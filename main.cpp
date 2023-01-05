#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
    string line;
    ifstream command_exit;
    
    system("cat /var/log/syslog | grep boot >> /tmp/syslog_read");
    
    command_exit.open ("/tmp/syslog_read");

    if (command_exit.is_open()){
        while (getline(command_exit, line)){
            cout << line << endl;
        }
        command_exit.close();
    }

    if (remove("/tmp/syslog_read") == 0)
        cout << "File removed succesfully" << endl;

    return 0;
}