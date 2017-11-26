package app.dao.po;

import java.io.Serializable;
import java.util.Date;

/**
 * c_config po
 * 
 * @author mybatis generator
 * @since 2016-11-03T19:45:56.463
 */
public class ConfigPO implements Serializable {

	/** 序列化ID */
	private static final long serialVersionUID = -1L;

	/** 配置ID */
	private Long configId;
	/**
	 * ‘模块’: 首页，订单，产品，等 0:无效 1.首页 2.订单 3.产品
	 */
	private Integer configModule;
	/** 配置key值 */
	private String configKey;
	/** 配置名称 */
	private String configName;
	/** 参数值数据类型, 1:整型 2:字符串 3:日期 4:枚举型 */
	private Integer valueType;
	/** 配置值 */
	private String configValue;
	/** 单位 */
	private String units;
	/** 状态(0:停用, 1:启用) */
	private Integer state;
	/**
	 * 操作人, 默认：系统 MIS：操作人 ID:名称, 系统[0:admin]
	 */
	private String operator;
	/** 备注 */
	private String remark;
	/** 创建时间 */
	private Date createTime;
	/** 最后修改时间 */
	private Date updateTime;

	/**
	 * 默认构造方法
	 */
	public ConfigPO() {

	}

	/**
	 * 主键构造方法
	 * 
	 * @param configId
	 *            配置ID
	 */
	public ConfigPO(Long configId) {
		this.configId = configId;
	}

	/**
	 * 获取 配置ID
	 * 
	 * @return 配置ID
	 */
	public Long getConfigId() {
		return configId;
	}

	/**
	 * 设置 配置ID
	 * 
	 * @param configId
	 *            配置ID
	 */
	public void setConfigId(Long configId) {
		this.configId = configId;
	}

	/**
	 * 获取 ‘模块’: 首页，订单，产品，等 0:无效 1.首页 2.订单 3.产品
	 * 
	 * @return ‘模块’: 首页，订单，产品，等 0:无效 1.首页 2.订单 3.产品
	 */
	public Integer getConfigModule() {
		return configModule;
	}

	/**
	 * 设置 ‘模块’: 首页，订单，产品，等 0:无效 1.首页 2.订单 3.产品
	 * 
	 * @param configModule
	 *            ‘模块’: 首页，订单，产品，等 0:无效 1.首页 2.订单 3.产品
	 */
	public void setConfigModule(Integer configModule) {
		this.configModule = configModule;
	}

	/**
	 * 获取 配置key值
	 * 
	 * @return 配置key值
	 */
	public String getConfigKey() {
		return configKey;
	}

	/**
	 * 设置 配置key值
	 * 
	 * @param configKey
	 *            配置key值
	 */
	public void setConfigKey(String configKey) {
		this.configKey = configKey;
	}

	/**
	 * 获取 配置名称
	 * 
	 * @return 配置名称
	 */
	public String getConfigName() {
		return configName;
	}

	/**
	 * 设置 配置名称
	 * 
	 * @param configName
	 *            配置名称
	 */
	public void setConfigName(String configName) {
		this.configName = configName;
	}

	/**
	 * 获取 参数值数据类型, 1:整型 2:字符串 3:日期 4:枚举型
	 * 
	 * @return 参数值数据类型, 1:整型 2:字符串 3:日期 4:枚举型
	 */
	public Integer getValueType() {
		return valueType;
	}

	/**
	 * 设置 参数值数据类型, 1:整型 2:字符串 3:日期 4:枚举型
	 * 
	 * @param valueType
	 *            参数值数据类型, 1:整型 2:字符串 3:日期 4:枚举型
	 */
	public void setValueType(Integer valueType) {
		this.valueType = valueType;
	}

	/**
	 * 获取 配置值
	 * 
	 * @return 配置值
	 */
	public String getConfigValue() {
		return configValue;
	}

	/**
	 * 设置 配置值
	 * 
	 * @param configValue
	 *            配置值
	 */
	public void setConfigValue(String configValue) {
		this.configValue = configValue;
	}

	/**
	 * 获取 单位
	 * 
	 * @return 单位
	 */
	public String getUnits() {
		return units;
	}

	/**
	 * 设置 单位
	 * 
	 * @param units
	 *            单位
	 */
	public void setUnits(String units) {
		this.units = units;
	}

	/**
	 * 获取 状态(0:停用, 1:启用)
	 * 
	 * @return 状态(0:停用, 1:启用)
	 */
	public Integer getState() {
		return state;
	}

	/**
	 * 设置 状态(0:停用, 1:启用)
	 * 
	 * @param state
	 *            状态(0:停用, 1:启用)
	 */
	public void setState(Integer state) {
		this.state = state;
	}

	/**
	 * 获取 操作人, 默认：系统 MIS：操作人 ID:名称, 系统[0:admin]
	 * 
	 * @return 操作人, 默认：系统 MIS：操作人 ID:名称, 系统[0:admin]
	 */
	public String getOperator() {
		return operator;
	}

	/**
	 * 设置 操作人, 默认：系统 MIS：操作人 ID:名称, 系统[0:admin]
	 * 
	 * @param operator
	 *            操作人, 默认：系统 MIS：操作人 ID:名称, 系统[0:admin]
	 */
	public void setOperator(String operator) {
		this.operator = operator;
	}

	/**
	 * 获取 备注
	 * 
	 * @return 备注
	 */
	public String getRemark() {
		return remark;
	}

	/**
	 * 设置 备注
	 * 
	 * @param remark
	 *            备注
	 */
	public void setRemark(String remark) {
		this.remark = remark;
	}

	/**
	 * 获取 创建时间
	 * 
	 * @return 创建时间
	 */
	public Date getCreateTime() {
		return createTime;
	}

	/**
	 * 设置 创建时间
	 * 
	 * @param createTime
	 *            创建时间
	 */
	public void setCreateTime(Date createTime) {
		this.createTime = createTime;
	}

	/**
	 * 获取 最后修改时间
	 * 
	 * @return 最后修改时间
	 */
	public Date getUpdateTime() {
		return updateTime;
	}

	/**
	 * 设置 最后修改时间
	 * 
	 * @param updateTime
	 *            最后修改时间
	 */
	public void setUpdateTime(Date updateTime) {
		this.updateTime = updateTime;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(getClass().getSimpleName());
		sb.append(" [");
		sb.append("Hash = ").append(hashCode());
		sb.append(", configId=").append(configId);
		sb.append(", configModule=").append(configModule);
		sb.append(", configKey=").append(configKey);
		sb.append(", configName=").append(configName);
		sb.append(", valueType=").append(valueType);
		sb.append(", configValue=").append(configValue);
		sb.append(", units=").append(units);
		sb.append(", state=").append(state);
		sb.append(", operator=").append(operator);
		sb.append(", remark=").append(remark);
		sb.append(", createTime=").append(createTime);
		sb.append(", updateTime=").append(updateTime);
		sb.append("]");
		return sb.toString();
	}
}