package app.dao;

import app.dao.po.ConfigPO;

/**
 * c_config mapper
 * 
 * @author mawentao
 * @since 2016-11-03T19:45:56.469
 */
public interface ConfigMapper {
	
	/**
	 * 获取表中记录总数
	 * @return
	 */
	public int getCount();
	
	/**
	 * 根据主键查找记录
	 * 
	 * @param id 主键
	 * @return 对应记录
	 */
	ConfigPO getById(Long id);
}