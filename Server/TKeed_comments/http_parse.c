#include "http.h"


int tk_http_parse_request_line(tk_http_request_t *request){
	    enum{
        sw_start = 0,
        sw_method,
        sw_spaces_before_uri,
        sw_after_slash_in_uri,
        sw_http,
        sw_http_H,
        sw_http_HT,
        sw_http_HTT,
        sw_http_HTTP,
        sw_first_major_digit,
        sw_major_digit,
        sw_first_minor_digit,
        sw_minor_digit,
        sw_spaces_after_digit,
        sw_almost_done
    }state;
    state = request->state;


    u_char ch, *p, *m;
    size_t	 pi;

    for(pi = request->pos; pi< request->last;pi++){
    	
    }

}