#include "include.hpp"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char *p;

	p = (unsigned char *)s;
	while (n--)
		*p++ = (unsigned char)c;
	return (s);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	nb;

	i = 0;
	sign = 1;
	nb = 0;
	while ((str[i] == '\n' || str[i] == '\t' || str[i] == '\v' || str[i] == '\r'
					|| str[i] == '\f' || str[i] == 32) && str[i])
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = sign * (-1);
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9') && str[i] != '\0')
	{
		nb = (nb * 10) + (str[i] - 48);
		i++;
	}
	return (nb * sign);
}

std::string ft_itoa(int n)
{
	std::string ret;

	std::ostringstream convert;
	convert << n;

	ret = convert.str();
	return (ret);
}

std::string		ft_getNextDir(std::string *path)
{
	size_t pos = path->find_last_of("/");
	*path = path->substr(0, pos);
	return *path;
}