#include <iostream>
#include <vector>
using namespace std;

/**
 * In the name of God
 * Homework 2
 * TODO: Complete this code using given TODO comments :). then remove TODOs
 * feel free to edit code as you want and make it better
 * Any questions? ask @devcom_support on telegram
 * Good luck!
 **/

enum UserType{
    ADMIN,
    MEMBER
};


class UserAlreadyExistsException : public exception {
    public :
    virtual const char* what () const throw(){
        return "this user already exists try again...-___-";
    }
}; //TODO: Give exceptions a better structure. search google (optional) :done ^^
class OopsException : public exception {
    public :
    virtual const char* what () const throw(){
        return "oops... username or password is not correct.. try again... -___-";
    } 
};


class AbstractUser{ // User structure
public:
    virtual bool authenticate(string username, string password) = 0;
   virtual void deleteAccount(vector<AbstractUser*> *users) {}; //TODO: 1. implement this in User class. (You can't compile code and create instance of User until then). DON'T TOUCH ABSTRACT USER! :done ^^
    string username;
protected:
    string password;
    UserType type;
};




class User : public AbstractUser{
public:

    User(string username, string password, UserType type){
        this->username = username;
        this->password = password;
        this->type = type;
    }

    bool authenticate(string username, string password){
        return this->username == username && this->password == password;
    }
    
    
    void deleteAccount(vector<AbstractUser*> *users) { 
        for(auto user=users->begin();user !=users->end();user++)
        {if(username ==(*user)->username){users->erase(user);
        break;
        }
    } 
        }

    static User* login(vector<AbstractUser*> *users, string username, string password){ //TODO: 2. handle user login errors with exceptions : done ^^
        for(auto user = users->begin(); user != users->end(); user++){
            if((*user)->authenticate(username, password)){
                return (User*) *user;
            }
        }
        OopsException e;
        throw e ;
    }

    static void signup(vector<AbstractUser*> *users, string username, string password){

        //Check if user with that username exists and throw UserAlreadyExistsException in that case
        for(auto user = users->begin(); user != users->end(); user++) { //TODO: 3. this doesn't work. fix it!! :done ^^
            if ((*user)->username == username) {
                UserAlreadyExistsException ex;
                throw ex;
            }
        }

        //Create user and add it to vector
        users->push_back(new User(username, password, UserType::MEMBER));
    }

    string username;
};

enum MenuState{
    START,
    LOGGED_IN,
    END
};

class AppDatabase { //Just holds runtime data. doesn't save anything
public:
    vector<AbstractUser *> appUsers;

    AppDatabase() { //Load initial data
        appUsers.push_back(new User("admin",
                                    "admin" /* password is unsafe! for test only */,
                                    UserType::ADMIN)
        );
    }
};

int main(){
    User * loggedInUser = nullptr;
    AppDatabase appDatabase;
    MenuState menuState = MenuState::START;

    char choice;
    cout << "Welcome!" << endl;

    while(menuState != MenuState::END){
        switch (menuState){
            case MenuState::START: {

                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case '1': {
                        string username, password;
                        cout << "Enter Username" << endl;
                        cin >> username;
                        cout << "Enter Password" << endl;
                        cin >> password;
                           try{
                        loggedInUser = User::login(&appDatabase.appUsers, username, password);
                            menuState = MenuState::LOGGED_IN;
                
                        }
                        catch(OopsException e)
                        {cout<<e.what()<<endl;}
                        break;
                    }
                    case '2': {
                        string username, password;
                        cout << "Enter Username" << endl;
                        cin >> username;
                        cout << "Enter Password" << endl;
                        cin >> password;
                        try{
                            User::signup(&appDatabase.appUsers, username, password);
                        } catch (UserAlreadyExistsException e) {
                            cout << "Error: username already exists";
                        }
                        break;
                    }
                    case 'e': {
                        menuState = MenuState::END;
                        break;
                    }
                    default: {
                        cout << "Unknown Input" << endl;
                    }
                }
                break;
            }
            case MenuState::LOGGED_IN: {
                cout << "d.delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case 'd': {
                        loggedInUser->deleteAccount(&appDatabase.appUsers);
                        cout << "Account successfully deleted";
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        break;
                    }
                    case 'l': {
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        break;
                    }
                    case 'e': {
                        menuState = MenuState::END;
                        break;
                    }
                    default: {
                        cout << "Unknown Input" << endl;
                    }
                }
                break;
            }
        }
    }

    return 0;

}

