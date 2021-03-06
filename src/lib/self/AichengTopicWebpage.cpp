// last modified 

#include "AichengTopicWebpage.h"
#include <iostream>
#include <iterator>
#include <algorithm>


using namespace std;

static bool
parsePicturesUrlsHelper ( const string& webpage_txt,
                          vector<string>& pictures_urls_list,
                          const string& begin_keyword,
                          const string& end_keyword ) 
{
    bool b_ok = false;

    size_t keyword_pic_begin_pos = 0, keyword_pic_end_pos = 0;
    while (true) {
        // parse picture URL
        keyword_pic_begin_pos = webpage_txt.find(begin_keyword, keyword_pic_end_pos);
        if (string::npos == keyword_pic_begin_pos) {
            break;
        }
        keyword_pic_end_pos = webpage_txt.find(end_keyword, keyword_pic_begin_pos + begin_keyword.size());
        if (string::npos == keyword_pic_end_pos) {
            //cerr << "WARNING! parsePicturesUrlsHelper() CANNOT find the keyword " << end_keyword << endl;
            return(false);
        }
        const string pic_url = webpage_txt.substr( keyword_pic_begin_pos + begin_keyword.size(),
                                                   keyword_pic_end_pos - keyword_pic_begin_pos - begin_keyword.size() );
        b_ok = true;
        
        // there are some bad picture-webspaces, ignore them
        bool bad_pic_websapce = false;
        static const vector<string> bad_pic_websapces_list = {/* "uploadfr.com", "picrar.com" */};
        for (const auto& e : bad_pic_websapces_list) {
            if (string::npos != pic_url.find(e)) {
                bad_pic_websapce = true;
                break;
            }
        }
        if (bad_pic_websapce) {
            continue;
        }
        
        // normal, gifs are AD
        bool b_gif = (string::npos != pic_url.rfind(".gif"));
        if (b_gif) {
            continue;
        }
        
        // save the picture URL
        pictures_urls_list.push_back(pic_url);
    }

    return(b_ok);
}

static bool
parsePicturesUrls (const string& webpage_txt, vector<string>& pictures_urls_list) 
{
    pictures_urls_list.clear();

    static const string begin_keyword0("<img src=\"");
    static const string end_keyword0("\"");
    if (parsePicturesUrlsHelper(webpage_txt, pictures_urls_list, begin_keyword0, end_keyword0)) {
        return(true);
    }

    static const string begin_keyword1("<img src=");
    static const string end_keyword1(" ");
    if (parsePicturesUrlsHelper(webpage_txt, pictures_urls_list, begin_keyword1, end_keyword1)) {
        return(true);
    }
    
    static const string begin_keyword2("<img src='");
    static const string end_keyword2(" ");
    if (parsePicturesUrlsHelper(webpage_txt, pictures_urls_list, begin_keyword2, end_keyword2)) {
        return(true);
    }
    

    return(false);
}

static bool
parseSeedUrl (const string& webpage_txt, string& seed_url) 
{
    static const string keyword_prefix("<a href=\"");
    static const vector<string> keywords_seed_begin_list = { "http://www.jandown.com", 
                                                             "http://jandown.com", 
                                                             "http://www6.mimima.com",
                                                             "http://mimima.com" };
    auto keyword_seed_begin_pos = string::npos;
    bool b_find_begin = false;
    for (const auto& e : keywords_seed_begin_list) {
        keyword_seed_begin_pos = webpage_txt.find(keyword_prefix + e);
        if (string::npos != keyword_seed_begin_pos) {
            b_find_begin = true;
            break;
        }
    }
    if (!b_find_begin) {
        //cerr << "WARNING! parseSeedUrl() cannot find any keywords " << keyword_prefix << " + : ";
        //copy( keywords_seed_begin_list.cbegin(), keywords_seed_begin_list.cend(),
              //ostream_iterator<const string&>(cerr, " ") );
        //cerr << endl;
        return(false);
    }

    static const string keyword_seed_end("\"");
    const auto keyword_seed_end_pos = webpage_txt.find( keyword_seed_end,
                                                        keyword_seed_begin_pos + keyword_prefix.size() );
    if (string::npos == keyword_seed_end_pos) {
        //cerr << "WARNING! parseSeedUrl() cannot find the keyword " << keyword_seed_end << endl;
        return(false);
    }

    seed_url = webpage_txt.substr( keyword_seed_begin_pos + keyword_prefix.size(),
                                   keyword_seed_end_pos - keyword_seed_begin_pos - keyword_prefix.size() );


    return(true);
}

AichengTopicWebpage::AichengTopicWebpage (const string& url, const string& proxy_addr)
    : TopicWebpage(url, parsePicturesUrls, parseSeedUrl, proxy_addr, "gbk", "UTF-8")
{
    ;
}

AichengTopicWebpage::~AichengTopicWebpage ()
{
    ;
}

