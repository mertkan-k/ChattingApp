#pragma once

#include <ctime>

class Core
{
public:
	// WSA Windows Socket Arc.
	void	StartWSA();
	void	CleanWSA();
	bool	IsWSAInitialized() const { return m_isWSAInit; }

	Core();
	virtual ~Core();
protected:
	time_t m_initTime;
	bool m_isWSAInit;
	
};