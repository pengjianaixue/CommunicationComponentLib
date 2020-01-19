#include "Telnet.h"


CTelnet::CTelnet(const std::string &address, const std::string &port,const std::string &userAccount /*= ""*/, const std::string &passwd/*=""*/)
{
	setConnectInfor(address, port, userAccount, passwd);
	//m_curl = curl_easy_init();
	
}
CTelnet::~CTelnet()
{
	/*curl_easy_cleanup(curl);
	curl_slist_free_all(options);*/
}

bool CTelnet::connect()
{
	if (m_curl) {

		/*options = curl_slist_append(NULL, "TTTYPE=vt100");
		options = curl_slist_append(m_options, std::string("USER=" + m_userAccount).c_str() );
		curl_easy_setopt(curl, CURLOPT_URL, std::string(m_address+ " " + m_port).c_str());
		curl_easy_setopt(curl, CURLOPT_TELNETOPTIONS, m_options);
		ret = curl_easy_perform(curl);
		return ret== CURLE_OK;*/
	}
	return false;
}

bool CTelnet::disconnect()
{
	return false;
}

bool CTelnet::send(const std::string & cmd)
{
	return false;
}

std::string CTelnet::recvi()
{
	return std::string();
}

void CTelnet::resetConnectInfor(const std::string &address, const std::string &port, const std::string &userAccount /*= ""*/, const std::string &passwd /*= ""*/)
{
	setConnectInfor(address, port, userAccount, passwd);
}

void CTelnet::setConnectInfor(const std::string &address, const std::string &port, const std::string &userAccount /*= ""*/, const std::string &passwd /*= ""*/)
{
	m_address = address;
	m_port = port;
	m_userAccount = userAccount;
	m_passwd = passwd;
}

bool CTelnet::login()
{
	return false;
}
