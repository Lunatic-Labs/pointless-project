// This is basic layout for automated email sender
// install curl library
// TO MAKE WORK YOU MUST:
// PUT CURL ONTO WEB BUILD SCRIPT
// it is a local thing for each device
// but sence we are make a build script
// it needs to be in the web build script
// to finish:
// import proper data from application
// ie. who has finished what process
// this would be taking data out of the data base for who has enter what code
// then generate report
// send to Dr. Towell at descrection
// ie. at end of day, once a week, twice a week etc. 

// must compile with -lcurl

//---------------------------------------------------------------------
// This must be compiled with main in src directory (does not exist)
//---------------------------------------------------------------------

//     pointless/
// ├── build/
// ├── Crow/
// ├── src/
// │   ├── main.cpp
// │   ├── EmailSender.h   # Email sender class declaration
// │   └── EmailSender.cpp
// └── CMakeLists.txt


#include <curl/curl.h>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>

class EmailSender {

private:

    const std::string FROM_EMAIL = "your-email@example.com";
    const std::string TO_EMAIL = "recipient@example.com";
    const std::string SMTP_SERVER = "smtp.example.com";
    const std::string USERNAME = "your-username";
    const std::string PASSWORD = "your-password";
    
    static const char* payload_text[] = {
        "To: " TO_EMAIL "\r\n",
        "From: " FROM_EMAIL "\r\n",
        "Subject: Daily Update\r\n",
        "\r\n",
        "This is your daily automated message.\r\n",
        NULL
    };

    // Function to memcopy from buffer data to consturct email to send 

    static size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp)
    {
        static int ptr_num = 0;
        const char* data = payload_text[ptr_num];

        if (data != NULL)
        {
            size_t len = strlen(data);
            memcpy(ptr, data, len);
            ptr_num++;
            return len;
        }
        return 0;
    }

public:

    bool sendEmail()
    {
        CURL* curl = curl_easy_init();
        if (!curl) return false;

        curl_easy_setopt(curl, CURLOPT_URL, SMTP_SERVER);

        curl_easy_setopt(curl, CURLOPT_USERNAME, USERNAME.c_str());

        curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD.c_str());

        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_EMAIL.c_str());
        
        struct curl_slist* recipients = NULL;

        recipients = curl_slist_append(recipients, TO_EMAIL.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        CURLcode res = curl_easy_perform(curl);
        
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
        
        return (res == CURLE_OK);
    }
};

int main() {
    curl_global_init(CURL_GLOBAL_ALL);
    EmailSender sender;

    // HERE IS WHERE INFO IS FILLED 
    sender.FROM_EMAIL = "pointless.hotmail.org";
    // This account doesn't exist will have to set up 
    sender.TO_EMAIL = "dwaynetowell@lipsomb.edu";
    // create and put in buffer
    // mem is placeholder
    sender.payload_source = (mem); 
    
    // construct message 
    // basic is already constructed
    sender.payload_text = {};
    
    
    while(true) {
        if(sender.sendEmail()) {
            std::cout << "Email sent successfully\n";
        } else {
            std::cout << "Failed to send email\n";
        }
        
        // Sleep for 24 hours
        // Change this to chane time in hours 
        std::this_thread::sleep_for(std::chrono::hours(24));
    }
    
    curl_global_cleanup();
    return 0;
}