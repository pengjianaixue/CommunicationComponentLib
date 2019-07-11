#include "SSHChannel.h"


Channel::Channel(LIBSSH2_CHANNEL *channel):m_channel(channel)
{

}



Channel::~Channel()
{
	if (m_channel)
	{
		libssh2_channel_free(m_channel);
		m_channel = nullptr;
	}
}

string Channel::Read(const string & strend, int timeout)
{
	string data;
	if (nullptr == m_channel)
	{
		return data;
	}
	LIBSSH2_POLLFD *fds = new LIBSSH2_POLLFD;
	fds->type = LIBSSH2_POLLFD_CHANNEL;
	fds->fd.channel = m_channel;
	fds->events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;

	if (timeout % 50)
	{
		timeout += timeout % 50;
	}
	while (timeout>0)
	{
		int rc = (libssh2_poll(fds, 1, 10));
		if (rc < 1)
		{
			timeout -= 50;
#ifdef _WIN32
			Sleep(50* 1000);
#else
			usleep(50 * 1000);
#endif // _WIN32
			continue;
		}
		if (fds->revents & LIBSSH2_POLLFD_POLLIN)
		{
			char buffer[64 * 1024] = "";
			size_t n = libssh2_channel_read(m_channel, buffer, sizeof(buffer));
			if (n == LIBSSH2_ERROR_EAGAIN)
			{
				//fprintf(stderr, "will read again\n");
			}
			else if (n <= 0)
			{
				return data;
			}
			else
			{
				data += string(buffer);
				if ("" == strend)
				{
					return data;
				}
				size_t pos = data.rfind(strend);
				if (pos != string::npos && data.substr(pos, strend.size()) == strend)
				{
					return data;
				}
			}
		}
		timeout -= 50;
#ifdef _WIN32
		Sleep(50* 1000);
#else
		usleep(50 * 1000);
#endif // _WIN32
	}
	std::cout << "read data timeout" << std::endl;
	return data;
}

bool Channel::Write(const string & data)
{
	if (nullptr == m_channel)
	{
		return false;
	}
	string send_data = data + "\n";
	return libssh2_channel_write_ex(m_channel, 0, send_data.c_str(), send_data.size()) == data.size();
}

