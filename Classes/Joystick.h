#ifndef __Joystick__
#define __Joystick__
 
#include "cocos2d.h"
USING_NS_CC;
 
enum JoystickEnum
{
    DEFAULT,
    D_UP,
    D_DOWN,
    D_LEFT,
    D_RIGHT,
    D_LEFT_UP,
    D_LEFT_DOWN,
    D_RIGHT_UP,
    D_RIGHT_DOWN
};
 
class Joystick : public Layer
{
    public:
        /** �����u���� */
        void onRun();
        /** ������� */
        void onDisable();
        /** ���������뾶���������뾶��ҡ����ʧЧ */
        void setDieRadius(float radius);
        /** ������Ч����뾶���������Ч����ȣ������ã� */
        void setFailRadius(float radius);
        /** �Ƿ���ʾ���̺ʹ��� */
        void setVisibleJoystick(bool visible);
        /** �Ƿ����ɱ任ҡ������λ�ã�������Ļ��ÿһ�ΰ������ʱ�����꽫��ҡ���������꣬�ƶ�ʱ�����ı�ҡ�������ֱ꣬���´ΰ������ */
        void setAutoPosition(bool value);
        /** �ص�����ָ�� */
        std::function<void(JoystickEnum)> onDirection;
        /** ��̬������������Ҫ������̺ʹ���ͼƬ·���� */
        static Joystick* create(std::string chassisPath,std::string dotPath);
        /** ��ʼ��ҡ��������Ҫ������̺ʹ���ͼƬ·���� */
        void initWithJoystick(std::string chassisPath,std::string dotPath);
    protected:
        /** ��Ч����뾶 */
        float _radius;
        /** ʧЧ����뾶 */
        float _failradius;
        /** �Ƿ��Ƴ���Ч���� */
        bool isMoveOut;
        /** �Ƿ������Ч����뾶 */
        bool isDieRadius;
        /** �Ƿ����ɱ任ҡ�������� */
        bool isAutoPosition;
        /** ���� */
        JoystickEnum _direction;
        /** ���� */
        Sprite* _chassis;
        /** ���� */
        Sprite* _touchDot;
        EventListenerTouchOneByOne* listener;
     
        bool onTouchBegan(Touch* touch,Event* event);
        void onTouchMoved(Touch* touch,Event* event);
        void onTouchEnded(Touch* touch,Event* event);
        /** 
        1�����ô��㣬���ж��Ƿ�����Ч�����ڣ��������Ч�����ڣ������ã�
        2�����ͽǶȱ仯�����������Ч�����ڣ� */
        void setTouchDotPosition(Vec2 vec1,Vec2 vec2);
        /** 
        1������ҡ�����˷���
        2�����ͽǶȱ仯���ص����ȱ仯���� */
        void changeAngle( Vec2 position );
        /** �ص�ע��ļ������� */
        void callDirectionFun();
        /** ���ã������Ȳ��� DEFAULTʱ�����ã� */
        void resetState();
     
};
 
#endif