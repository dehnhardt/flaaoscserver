#ifndef OSCSENDER_H
#define OSCSENDER_H

#include <QThread>

class OscSender : public QThread
{
public:
	OscSender( int iPortNum);

protected:
	void run() override;

private:
	void runSender();

private:
	int m_iPortNum = 0;
};

#endif // OSCSENDER_H
