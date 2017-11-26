package app.ctrl;

import javax.servlet.http.HttpServletRequest;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.RequestMapping;

/**
 * BaseController
 * 
 * @author _AUTHOR_
 * @since _DATETIME_
 */

@RestController
public class BaseController {

    @RequestMapping("/base")
    public String hello(HttpServletRequest request){
        return "Hello BaseController";
    }

}
