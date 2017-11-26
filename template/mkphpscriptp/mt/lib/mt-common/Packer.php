<?php

/***********************************************************
 * @file:    Packer.php
 * @version: 1.0.0
 * @author:  mawentao(mawentao@baidu.com)
 * @brief:   Packer.php
 ***********************************************************/

class Packer {
	protected $_szData;			// Data
    protected $_iDataLen;		// Data Len
    protected $_iPos;			// Data Pos
    protected $_bLittleEndian;	// Little Endian
	
	// Construct Function
    public function __construct($bolLittleEndian = true) {
        $this->clearData();
        $this->_bLittleEndian = $bolLittleEndian;
    }


	// Get Data
    public function getData() {
        return $this->_szData;
    }

	// Get Data Len
	public function getLen() {
        return $this->_iDataLen;
	}

	// Get Data Pos
    public function getPos() {
        return $this->_iPos;
    }

	// Set Data Pos
	public function setPos($iPos) {
        if ($iPos < $this->_iDataLen) {
            $this->_iPos = $iPos;
        }
	}

	// Get Remain Data Len
    public function getRemainLen() {
        return $this->_iDataLen - $this->_iPos;
	}

	// Get Remain Data
    public function getRemainData() {
        if ($this->_iPos < $this->_iDataLen) {
            $intLen = $this->_iDataLen - $this->_iPos;
            return substr($this->_szData, $this->_iPos, $intLen);
        } else {
            return '';
        }
    }

	// Set Data
	public function setData($strData) {
        $this->clearData();
        $this->_szData = $strData;
        $this->_iDataLen = strlen($strData);
    }

	// Clear Data
	public function clearData() {
        $this->_szData = '';
        $this->_iPos = 0;
        $this->_iDataLen = 0;
	}

	////////////////////
	//
	// Pack Function
	//
	////////////////////

	// Pack Int8
	public function packInt8($intVal8) {
        $intVal8 = chr($intVal8);
        $intVal8 = pack('a', $intVal8);
        $this->_szData.= $intVal8;
        $this->_iDataLen+= 1;
	}

	// Pack Int16
    public function packInt16($intVal16) {
        $this->_szData.= pack('S', $intVal16);
        $this->_iDataLen+= 2;
	}

	// Pack Int32
    public function packInt32($intVal32) {
        $this->_szData.= pack('I', $intVal32);
        $this->_iDataLen+= 4;
	}

	// Pack Int64
    public function packInt64($intVal64) {
        $intH32 = (($intVal32 >> 32) & (0x00000000FFFFFFFF));
        $intH32 = intval($intH32);
        $intL32 = (($intVal32) & (0x00000000FFFFFFFF));
        $intL32 = intval($intL32);
        if ($this->_bLittleEndian) { //little endian
            $this->_szData.= pack('V', $intL32);
            $this->_szData.= pack('V', $intH32);
        } else { //big endian
            $this->_szData.= pack('N', $intH32);
            $this->_szData.= pack('N', $intL32);
        }
        $this->_iDataLen+= 8;
	}

	// Pack N Char
    public function packNChar($strValue, $intNum) {
        $this->_szData.= pack('a' . $intNum, $strValue);
        $this->_iDataLen+= $intNum;
	}

	////////////////////
	//
	// UnPack Function
	//
	////////////////////


	// UnPack Int8
    public function unpackInt8() {
        $strTmpData = substr($this->_szData, $this->_iPos, 1);
        $strTmpData = unpack('a', $strTmpData);
        $this->_iPos++;
        $intR8 = $strTmpData[1];
        $intR8 = ord($intR8);
        return $intR8;
	}

	// UnPack Int16
    public function unpackInt16() {
        $strTmp = substr($this->_szData, $this->_iPos, 2);
        $this->_iPos+= 2;
        $arrData = unpack("S", $strTmp);
        return $arrData[1];
	}

	// UnPack Int32
    public function unpackInt32() {
        $strTmpData = substr($this->_szData, $this->_iPos, 4);
        $this->_iPos+= 4;
        $arrData = unpack("I", $strTmpData);
        return $arrData[1];
	}

	// UnPack Int64
    public function unpackInt64() {
        $strLData = substr($this->_szData, $this->_iPos, 4);
        $this->_iPos+= 4;
        $strHData = substr($this->_szData, $this->_iPos, 4);
        $this->_iPos+= 4;
        $intR64 = 0;
        if ($this->_bLittleEndian) { //little endian
            $arr = unpack('VL32', $strLData);
            $intL32 = $arr[L32];
            $arr = unpack('VH32', $strHData);
            $intH32 = $arr[H32];
            $intR64 = (($intH32 << 32) & (0xFFFFFFFF00000000));
            $intR64+= $intL32;
            return $intR64;
        } else { //big endian
            $arr = unpack('NH32', $strLData);
            $intH32 = $arr[H32];
            $arr = unpack('NL32', $strHData);
            $intL32 = $arr[L32];
            $intR64 = (($intH32 << 32) & (0xFFFFFFFF00000000));
            $intR64+= $intL32;
            return $intR64;
        }
	}
	
	// UnPack N Char
    public function unpackNChar($intN) {
        $strTmpData = substr($this->_szData, $this->_iPos, $intN);
        $this->_iPos+= $intN;
        $arrData = unpack("C*", $strTmpData);
        $strResData = '';
        foreach ($arrData as $data) {
            $strResData.= chr($data);
        }
        return trim($strResData);
	}
}
?>
