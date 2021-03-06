#include "device.h"

#include "QKindleFb/qkindlefb.h"

const Device::Properties Device::PROPS[] = {
//    x    y    DPI, KBD    JOY
    {600, 800,  167, false, false},    // UNKNOWN
    {600, 800,  167, true,  true },    // EMULATOR
    {600, 800,  167, true,  true },    // K2
    {824, 1200, 152, true,  true },    // KDX
    {600, 800,  167, true,  true },    // K3
    {600, 800,  167, false, true },    // K4NT
    {600, 800,  167, false, true },    // K4NTB
    {600, 800,  167, false, false},    // KT
    {758, 1024, 212, false, false},    // KPW
};

Device::Model Device::m_model = UNKNOWN;
int Device::version = 0;
    
Device::Device()
{
#ifdef i386
    m_model = EMULATOR;
    return;
#endif
    QStringList list;
    QProcess *myProcess = new QProcess();

    list << "-c" << "grep erial /proc/cpuinfo|cut -c12-15";
    myProcess->start("/bin/sh", list);
    if (myProcess->waitForReadyRead(10000)) {
        QByteArray array = myProcess->readAll();
        array.truncate(array.indexOf("\n"));

        bool ok;
        int sn = QString(array).toInt(&ok, 16);

        if (ok) {
            qDebug("serial: %X", sn);
        } else {
            qDebug() << "unexpected output: " << QString(array);
            return;
        }

        switch(sn) {
        case 0xB002:
        case 0xB003:
            m_model = K2; // may not work as K2 doesn't print SN in cpuinfo
            break;
        case 0xB004:
        case 0xB005:
        case 0xB009:
            m_model = KDX;
            break;
        case 0xB006:
        case 0xB008:
        case 0xB00A:
            m_model = K3;
            break;
        case 0xB00E:
            m_model = K4NT;
            break;
        case 0xB00F:
        case 0xB010:
        case 0xB011:
        case 0xB012: // ???
            m_model = KT;
            break;
        case 0x9023:
        case 0xB023:
            m_model = K4NTB;
            break;
        case 0xB01B:
        case 0xB01C:
        case 0xB01D:
        case 0xB01F:
        case 0xB024:
            m_model = KPW;
            break;
        default:
            qDebug("Unknown model: %X", sn);
        }
    }


    myProcess->close();
}

Device &Device::instance()
{
    static Device instance;
    // qDebug("DEVICE: %d (%d x %d @ %d) [%d|%d]", getModel(), getWidth(), getHeight(), getDpi(), hasKeyboard(), hasFiveWay());
    return instance;
}

void Device::suspendFramework(bool fast)
{
#ifdef BUILD_FOR_ARM
    qDebug("- framework");
    if (!isTouch()) {
        // this pause lets CVM handle painting before stopping, or screensaver may not draw
        // on next resume when device is turned off
        sleep(1);
        QProcess::execute("killall -STOP cvm");
    } else {
        QProcess::execute(QString("/bin/sh /var/tmp/ktsuspend.sh %1").arg(fast ? 1 : 0));
    }
    QWSServer::instance()->enablePainting(true);
#endif
}

void Device::resumeFramework(bool fast)
{
#ifdef BUILD_FOR_ARM
    qDebug("+ framework");
    QWSServer::instance()->enablePainting(false);
    if (!isTouch()) {
        QProcess::execute("killall -CONT cvm");
    } else {
        QProcess::execute(QString("/bin/sh /var/tmp/ktresume.sh %1").arg(fast ? 1 : 0));
    }
#endif
}

void Device::enableInput(bool enable)
{
#ifdef BUILD_FOR_ARM
    if (enable) {
        isTouch() ? QWSServer::instance()->openMouse() : QWSServer::instance()->openKeyboard();
    } else {
        isTouch() ? QWSServer::instance()->closeMouse() : QWSServer::instance()->closeKeyboard();
    }
#endif
}

void Device::setFullScreenUpdateEvery(int n)
{
#ifdef BUILD_FOR_ARM
    QKindleFb *pscreen = static_cast<QKindleFb*>(QScreen::instance());
    if (pscreen) pscreen->setFullUpdateEvery(n);
#endif
}

void Device::forceFullScreenUpdate(bool fullScreen)
{
#ifdef BUILD_FOR_ARM
    QKindleFb *pscreen = static_cast<QKindleFb*>(QScreen::instance());
    if (pscreen) pscreen->forceFullUpdate(fullScreen);
#endif
}

int Device::getTouchVersion()
{
    if(Device::version != 0)
		return Device::version;

	Device::version = 500;
	FILE *fp = fopen("/etc/version.txt","r");
	if(!fp)
	{
		return Device::version;
	}

	const char *yoshi = "yoshi-";
	char *find;
	char buff[128] = {0};

	fread(buff,1,sizeof(buff) - 1,fp);
	fclose(fp);
	find = strstr(buff, yoshi);
	if(find == NULL)
		return Device::version;
	
	int iversion = atoi(find + strlen(yoshi));

	if (iversion >= 180429) 
	{
		Device::version = 532;
	}
	else if (iversion >= 167953) 
	{
		Device::version = 512;
	}
	else if (iversion >= 156819) 
	{
		Device::version = 511;
	}
	else if (iversion >= 155776) 
	{
		Device::version = 510;
	}
	else if (iversion >= 149604) 
	{
		Device::version = 504;
	}
	else if (iversion >= 137371) 
	{
		Device::version = 501;
	}
	else if (iversion >= 137028) 
	{
		Device::version = 500;
	}	
	return Device::version;
}

bool Device::isTouch510()
{
    return (getTouchVersion() >= 510);
}

