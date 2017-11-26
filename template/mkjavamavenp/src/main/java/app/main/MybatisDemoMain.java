package app.main;

import org.springframework.context.support.ClassPathXmlApplicationContext;
import app.bean.G;
import app.dao.ConfigMapper;
import app.dao.po.ConfigPO;

/**
 * MybatisDemo程序入口
 * @author _AUTHOR_
 * @date _DATETIME_
 **/

public class MybatisDemoMain {

	public static void main(String[] args) throws Exception {
		// 加载Spring配置
		ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("spring-main.xml");
		
		try {
			G g = context.getBean(G.class);
			System.out.println(g.getVersion());
		
			// 获取数据表mapper
			ConfigMapper configMapper = context.getBean(ConfigMapper.class);
			
			// 查询表中记录总数
			System.out.println("表中记录总数："+configMapper.getCount());
			
			// 查询ID为1的记录
			ConfigPO configPO = configMapper.getById((long)1);
			System.out.println(configPO);
			
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		// 关闭SpringContext
		context.close();
		System.out.println("END");	
	}
}
