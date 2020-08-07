/*
 *  Project 1 CS251
 * programmer: AKSHAJ UPPALA
 * 
 * The programs aim is to load a spamlist and allot it into a vector.
 * The program also displays all the emails in the spam list.
 * It can help you check if an email is a spam email.
 * When provided with a file containing emails, 
 * it filters out the nonspam emails and puts it into a different file
 */


#include <iostream>
#include <fstream>
#include <string>
#include "ourvector.h"
using namespace std;

//Helps in loading the emails in the given file to the vectors domains and usernames
//parameters are:
//filename - file containing spam list
//domains - vetor which will contain the domains of spam list
//usernames - vector which will contain the usernames of the spam list
void load(string filename, ourvector<string>& domains, ourvector<string>& usernames){
    
    int count = 0;
    string spamMail;    //each spammail in the spam file
    int colonPosition;  // takes the poistion of the colon in the spam mail
    
    ifstream infile;
    infile.open(filename);
    
    if (!infile.good()){
        
        cout << "**Error, unable to open '" << filename << "'" << endl << endl;
        return;
    }
    else{
        
        cout << "Loading '" << filename << "'" << endl;
        
        infile >> spamMail;
        while(!infile.eof()){
            
            colonPosition = spamMail.find(':');
            domains.push_back(spamMail.substr(0, colonPosition));
            usernames.push_back(spamMail.substr(colonPosition+1, spamMail.size()-colonPosition-1));
            count++;
            infile >> spamMail;
        }
        infile.close();
        cout << "# of spam entries: " << count << endl << endl;
        return;
    }
    
}
    
//displays all the emails in the spamlist
//parameters are:
//domains - vetor which contains the domains of spam list
//usernames - vector which contains the usernames of the spam list
void display(ourvector<string> domains, ourvector<string> usernames){
    
    for(int i = 0; i < domains.size(); ++i){
        
        cout << domains.at(i) << ":" << usernames.at(i) << endl;
    } 
    cout << endl;
}

//searches for a word in the vector of strings
//parameters are:
//word - the string to be searched
//words - the vector containing the list of strings
//returns -1 if the string is not found or returns the position of the string in domains vector
int binarySearch(string word, ourvector<string>& words)
{
    int left = 0;
    int right = words.size()-1;
    int mid;
    string midElement;
    
    while(left<=right ){
        
        mid = left + (right-left)/2;
        midElement = words.at(mid);
         
        if(midElement == word){
            
            //while loop to find the first instance of domain occurence in the domains vector.
            //as the if statement can only find one of the domains
            while(mid!=0 && words.at(mid-1) == word){
                mid = mid - 1;                     
            }
            return mid;
        }
        else if(word>midElement){
            left = mid+1;
        }
        else if (word< midElement){
            right = mid-1;
        }
       
    }
    
  	return -1;
}

//checks if the given email is in the spam list
//parameters are:
//userFile - the email to be checked if it is spam
//domains - vetor which contains the domains of spam list
//usernames - vector which contains the usernames of the spam list
bool check(string userFile, ourvector<string>& domains, ourvector<string>& usernames){
    
    int atPosition = userFile.find("@");
    string userUsername = userFile.substr(0, atPosition);
    string userDomain = userFile.substr(atPosition+1, userFile.size()-atPosition-1);
    int searchResult = binarySearch(userDomain, domains);
    
    if(searchResult == -1){
        return false;
    }
    else{
        //loop to search for the corresponding username for the domain that has been found
        for(int i = searchResult; i < domains.size(); ++i){
            
            if(domains.at(i)==userDomain && (usernames.at(i)==userUsername || usernames.at(i)=="*")){
                return true;
            }
            
        }
     
        return false;
    }
}

//removes the spam mails from the list of given emails and outputs the filtered mails into an output file
//filename - the name of file containing the mails to be filtered
//outputFile - the file which will contain the nonspam files after filtering
//domains - vetor which contains the domains of spam list
//usernames - vector which contains the usernames of the spam list
void filter(string filename, string outputFile, ourvector<string> domains, ourvector<string> usernames){
    
    int ID;
    string Mail;
    string subject;
    ifstream infile;
    ofstream outfile;
    int numOfEmails = 0;
    int numOfNonSpamEmails = 0;
    infile.open(filename);
    outfile.open(outputFile);
    
    if (!infile.good()){
        
        cout << "**Error, unable to open '" << filename << "'" << endl << endl;
        return;
    }
    else if(!outfile.good()){
        
        cout << "**Error, unable to open '" << outputFile << "'" << endl << endl;
        return;
    }
    else{
        
        infile >> ID;
        infile >> Mail;
        getline(infile, subject);
        
        while(!infile.eof()){

            if(!check(Mail, domains, usernames)){
                
                //outputting the nonspam email into the outputfile using ostream
                outfile << ID << " " << Mail << subject << endl;
                numOfNonSpamEmails++;
            }
            
            numOfEmails++;
            infile >> ID;
            infile >> Mail;
            getline(infile, subject);
        }
        infile.close();
        outfile.close();
    
        cout << "# emails processed: " << numOfEmails << endl;
        cout << "# non-spam emails: " << numOfNonSpamEmails << endl;
        cout << endl;
    }
}


int main() {
    
    string command;
    string filename;
    string outputFile = "";
    ourvector<string> domains;
    ourvector<string> usernames;
    
    cout << "** Welcome to spam filtering app **" <<endl << endl;
    
    cout << "Enter command or # to exit> ";
    cin >> command;
    
    while(command != "#"){
    
        if(command == "load"){
            cin >> filename;
            domains.clear();
            usernames.clear();
            
            load(filename, domains, usernames);
        }
        else if(command == "display"){
            
            display(domains, usernames);
        }
        else if (command == "check"){
            
            cin >> filename;
            if(check(filename, domains, usernames)){
                cout << filename << " is spam" << endl << endl;
            }
            else {
                cout << filename << " is not spam" << endl << endl;
            }
        }
        else if (command == "filter"){
            
            cin >> filename;
            cin >> outputFile;
            
            filter(filename, outputFile, domains, usernames);
        }
        else{
        
        cout << "**invalid command" << endl << endl; 
        }
    
    cout << "Enter command or # to exit> ";
    cin >> command;
    
    }
    
    return 0;
}