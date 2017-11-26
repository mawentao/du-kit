import sys
import traceback
from types import StringType
import urlparse
import urllib2
import datetime
import lxml.html

def deal_str(str):
	return str.encode('utf-8').replace("\"","").replace("'","").replace("\r","").replace("\n","").replace("\t","").replace(" ","")

def deal_obj(obj):
	if isinstance(obj, lxml.html.HtmlElement):
		return deal_str(obj.text_content());
	else:
		return deal_str(obj)

#transform an unicode obj to string
def obj2str( iterable,sc='gbk',dc='gbk'):
    if isinstance( iterable, dict):
        #print '{-->dict'
        return '{'+ dict2str( iterable,sc,dc ) + '}'
    elif isinstance( iterable, list):
#print '[-->list',iterable
        return '['+  list2str( iterable,sc,dc ) + ']'
    elif isinstance( iterable, tuple):
        #print '(-->tuple'
        return  '(' + tuple2str( iterable,sc,dc ) + ')'
    elif isinstance( iterable, str):
        if sc == dc:
            return  "\""+str(iterable)+"\""
        else :
            return "\""+str(iterable).decode(sc).encode(dc)+"\""
    else:
        return str(iterable)
            
            
def dict2str( nest,sc='gbk',dc='gbk' ):
    strtmp=''
    for item in nest:
        strtmp += obj2str( item,sc,dc)+':'+ obj2str( nest[item],sc,dc )+','
    return strtmp[:-1]
    
def list2str( nest,sc='gbk',dc='gbk' ):
    strtmp=''
    for item in nest:
        strtmp += obj2str(item,sc,dc ) + ","
    return strtmp[:-1]
           
def tuple2str(nest,sc='gbk',dc='gbk'):
    strtmp=''
    for item in nest:
        strtmp=obj2str(item,sc,dc) + ","
    return strtmp[:-1]

#transform the sem url into real target url
#e.g. http://www.xg4gen.com/redir.html?target=http://www.suxin.com => http://www.suxin.com
def sem2target(sem_url):
    sem_url = sem_url.lower()
    idx1 = sem_url.find("http://",5)
    if idx1 > 0:
        url = sem_url[idx1:]
        return url
    idx2 = sem_url.find("http%3a%2f%2f",5)
    if idx2 > 0:
        url = urllib2.unquote(sem_url[idx2:])
        return url
    return sem_url

#get url domain
#e.g. getDomain("http://www.xg4gen.com/redir.html") => xg4gen.com
def getDomain( url ):
    dotjoint = lambda x: '.'.join(x)
    site = getSite( sem2target(url) )
    dlvl_list = site.split('.')
    if region_d.has_key(dlvl_list[-1]):
        if category_d.has_key(dlvl_list[-2]):
            return dotjoint(dlvl_list[-3:])
        else:
            return dotjoint(dlvl_list[-2:])
    else:
        if category_d.has_key(dlvl_list[-1]):
            return dotjoint(dlvl_list[-2:])
    return dotjoint(dlvl_list[1:])

#get url site
#e.g. getSite("http://www.xg4gen.com/redir.html") => www.xg4gen.com
def getSite(url):
    if not url.startswith("http"): 
        url = "http://"+url
    url_info = urlparse.urlparse( url )
    array = url_info[1].split("?")
    site = array[0].replace("\"","")
    return site

#transform a feature json string into a python dict
def str2feature(json_str):
    try:
        #print>>sys.stderr, "%s" % json_str
        feat_obj = eval(json_str.strip("]").strip("[") )
        for key, value in feat_obj.iteritems():
            if type(value) is StringType:
                feat_obj[key] = value.replace("\/","/").replace("\n"," ").replace("\t"," ").replace("\r"," ")
        return feat_obj
    except Exception,e:
        print e
        return None


