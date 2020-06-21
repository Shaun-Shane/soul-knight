#ifndef _FlowWord_H_
#define _FlowWord_H_

#include "cocos2d.h"
USING_NS_CC;

class FlowWord : public Node {
public:
	CREATE_FUNC(FlowWord);

	virtual bool init();

public:
	void showWord(int num, Point pos);
private:
	Label* m_textLab;
};

#endif