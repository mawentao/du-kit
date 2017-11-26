package app.main;

import org.apache.log4j.Logger;
import app.util.TimeTool;

/**
 * 程序启动入口
 * @author _AUTHOR_
 * @date _DATETIME_
 */

public class Main {

    private static Logger logger = Logger.getLogger(Main.class);

	public static void main(String[] args) {
        long startTime = System.currentTimeMillis();
        logger.info("program_start");

		System.out.println("Hello World");

        logger.info(String.format("program_stop [耗时:%s]", TimeTool.getTimeDiff(startTime)));
	}

}
