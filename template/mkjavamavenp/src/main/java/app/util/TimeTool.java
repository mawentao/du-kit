package app.util;

/**
 * 时间处理工具类
 * @author _AUTHOR_
 * @date _DATETIME_
 */

public class TimeTool {

	/**
	 * 获取当前时间戳
	 * @return
	 */
	public static long getNowTimestamp() {
		return System.currentTimeMillis()/1000;
	}
	
	/**
	 * 计算耗时（起始时间距离现在的耗时）
	 * @param beginTime 起始时间戳 System.currentTimeMillis()
	 * @return
	 */
	public static String getTimeDiff(long beginTime) {
		long timeDiff = System.currentTimeMillis() - beginTime;
		if (timeDiff>1000) {
			double secondDiff = (double)timeDiff/1000;
			return String.format("%s s", secondDiff);
		}
		return String.format("%s ms", timeDiff);
	}

}
