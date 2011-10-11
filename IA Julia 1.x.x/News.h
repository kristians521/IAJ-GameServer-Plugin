// ================================================== //
// #			GameServer 1.00.90					# //
// #			Imagination Arts					# //
// #			Julia Project 1.1.x					# //
// ================================================== //
// #	http://imaginationarts.net/forum/			# //
// #	http://auroraplay.ru/						# //
// ================================================== //

#ifndef NEWS_H
#define NEWS_H

void NewsSystem(void * lpParam);
class cNews
{
public:
	cNews();
	~cNews();  
	int Sections;
	struct sNews
	{
		char MSG01[50];
		char MSG02[50];
		char MSG03[50];
		float Time;
		int Type;
	};
	sNews NewsSys[20];
};
extern cNews News;
#endif