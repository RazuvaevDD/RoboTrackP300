/*
 * Этот объект проектирует цифровые фильтры и фильтрует потоки цифровых данных
 * 
 * Как использовать:
 * Вызвать объект типа Filter. Доступны 2 конструктора.
 * Один используется для фильтров LPF и HPF, другой - для фильтров BPF.
 * Аргументы для конструкторов следующие:
 * 
 * 		// Только для LPF или HPF:
 * 		Filter(filterType filt_t, int num_taps, double Fs, double Fx);
 * 		// Только для BPF:
 * 		Filter(filterType filt_t, int num_taps, double Fs, double Fl, double Fu);
 * 
 * Аргументы:
 * filt_t: тип фильтра: LPF, HPF или BPF (LPF - низкочастотный фильтр, HPF - высокочастотный фильтр, BPF - полосовой фильтр)
 * num_taps: количество нажатий, которые вы хотите использовать фильтр
 * Fs: частота дискретизации фильтруемых цифровых данных
 * Fx: частота перехода для фильтров LPF и HPF
 * Fl, Fu: верхняя и нижняя частоты перехода для фильтров BPF
 * 
 * После создания фильтра вы можете начать фильтрацию данных. 
 * Вот пример 51-полосной низкочастотной фильтрации потока с 
 * частотой дискретизации 44,1 кГц (частота дискретизации CD), 
 * где целью является создание сигнала «телефонной» полосы пропускания (4 кГц):
 * 
 * Filter *my_filter;
 * 
 * my_filter = new Filter(LPF, 51, 44.1, 4.0)
 * if( my_filter->get_error_flag() != 0 ) // abort in an appropriate manner
 * 
 * while(data_to_be_filtered){
 * 	next_sample = // Get the next sample from the data stream somehow
 * 	filtered_sample = my_filter->do_sample( next_sample );
 *   .
 * 	.
 * 	.
 * }
 * delete my_filter;
 * 
 * Предусмотрено несколько вспомогательных функций:
 *     init(): Фильтр можно повторно инициализировать с помощью вызова этой функции.
 *     get_taps(double *taps): returns the filter taps in the array "taps"
 *     write_taps_to_file(char *filename): записывает фильтр в файл
 *     write_freqres_to_file(char *filename): выводит частотную характеристику в файл
 * 
 * Наконец, предоставляется функция get_error_flag ().  Рекомендуется использовать 
 * для проверки возвращаемого значения: get_error_flag () должно возвращать нулевое значение после создания нового объекта Filter.  
 * Если оно ненулевое, посмотрите на следующую таблицу, чтобы понять смысл ошибки:
 * -1:  Fs <= 0
 * -2:  Fx <= 0 or Fx >= Fs/2
 * -3:  num_taps <= 0 or num_taps >= MAX_NUM_FILTER_TAPS
 * -4:  Не удалось выделить память для необходимых массивов
 * -5:  неверный filterType был передан в конструктор
 * -10: Fs <= 0 (при выборе BPF)
 * -11: Fl >= Fu
 * -12: Fl <= 0 || Fl >= Fs/2
 * -13: Fu <= 0 || Fu >= Fs/2
 * -14: num_taps <= 0 or num_taps >= MAX_NUM_FILTER_TAPS (при выборе BPF)
 * -15:  Не удалось выделить память для необходимых массивов (при выборе BPF)
 * -16:  неверный filterType был передан в конструктор (при выборе BPF)
 * 
 * Note that if a non-zero error code value occurs, every call to do_sample()
 * will return the value 0. write_taps_fo_file() will fail and return a -1 (it
 * also returns a -1 if it fails to open the tap file passed into it).
 * get_taps() will have no effect on the array passed in if the error_flag
 * is non-zero. write_freqres_to_file( ) returns different error codes
 * depending on the nature of the error...see the function itself for details.
 * 
 * Фильтры разработаны с использованием «метода рядов Фурье». 
 * Это означает, что коэффициенты приближения ряда Фурье к частотной характеристике 
 * идеального фильтра (LPF, HPF, BPF) используются в качестве отводов фильтра. 
 * Получающиеся фильтры имеют некоторую пульсацию в полосе пропускания из-за явления Гиббса; 
 * фильтры линейной фазы.
 */

#ifndef _FILTER_H
#define _FILTER_H

#define MAX_NUM_FILTER_TAPS 1000

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

enum filterType {LPF, HPF, BPF};

class Filter{
	private:
		filterType m_filt_t;
		int m_num_taps;
		int m_error_flag;
		double m_Fs;
		double m_Fx;
		double m_lambda;
		double *m_taps;
		double *m_sr;
		void designLPF();
		void designHPF();

		// Only needed for the bandpass filter case
		double m_Fu, m_phi;
		void designBPF();

	public:
		Filter(filterType filt_t, int num_taps, double Fs, double Fx);
		Filter(filterType filt_t, int num_taps, double Fs, double Fl, double Fu);
		~Filter( );
		void init();
		double do_sample(double data_sample);
		int get_error_flag(){return m_error_flag;};
		void get_taps( double *taps );
};

#endif
