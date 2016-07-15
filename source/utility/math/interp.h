#pragma once
#include "types.h"
#include "time/util_time.h"
#include "helper/func.h"
#include "util_math.h"

namespace util
{
	namespace math
	{
		class Interpolator
		{
		public: //interpolator functors
			struct Linear{ float operator () (float beg, float end, Time curr_time, Time lerp_time); };
			struct EaseInExpo
			{
				int expo;
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseOutExpo
			{
				int expo;
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseInOutExpo
			{
				int expo;
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseInCirc
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseOutCirc
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseInOutCirc
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseInSine
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseOutSine
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseInOutSine
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseInElastic
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseOutElastic
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseInBack
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseOutBack
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct EaseInOutBack
			{
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};


			struct Exponential
			{
				int exponent;
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			struct Overshoot
			{
				float pass_point_ratio; //ex: 0.3
				float over_shoot_ratio; //ex: 0.25
				float operator () (float beg, float end, Time curr_time, Time lerp_time);
			};
			__todo() // create some more interpolators

				/*Data*/
		private:
			std::function<float(float, float, Time, Time)> m_pInterpFunc;
			std::function<void()> m_pFinishFunc;
			float m_fBeg;
			float m_fLerp;
			float m_fEnd;
			Time m_fCurrTime;
			Time m_fLerpTime;
			bool m_bIsInterpolating;

			/*Func*/
		private:
			void GotoEnd(void);
		public:
			Interpolator(void);

			void Init(const float& beg, const float& end, const float& lerp_time,
				const std::function<float(float, float, Time, Time)> interp_func = Linear(),
				const std::function<void(void)> pFinishFunc = nullptr);
			void Update(const Time& fDelta);
			void Finish(const Time& fTime = 0);
			void Reset(void);

			float Poll(void) const { return this->m_fLerp; }

			inline void Play(void) { this->m_bIsInterpolating = true; }
			inline void Pause(void) { this->m_bIsInterpolating = false; }
			inline bool IsInterpolating(void) const { return this->m_bIsInterpolating; }

			//Gets
			inline float GetBeg(void) const { return this->m_fBeg; }
			inline float GetEnd(void) const { return this->m_fEnd; }
			inline Time GetCurrTime(void) const { return this->m_fCurrTime; }
			inline Time GetLerpTime(void) const { return this->m_fLerpTime; }
			//Sets
			inline void SetBeg(const float& fBeg) { this->m_fBeg = fBeg; }
			inline void SetEnd(const float& fEnd) { this->m_fEnd = fEnd; }
			inline void SetCurrTime(const Time& fCurrTime) { this->m_fCurrTime = fCurrTime; }
			inline void SetLerpTime(const Time& fLerpTime) { this->m_fLerpTime = fLerpTime; }
			inline void SetInterpFunc(const std::function<float(float, float, Time, Time)> interp_func) { this->m_pInterpFunc = interp_func; }
		};
	}
}