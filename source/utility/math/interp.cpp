#include "math/interp.h"
#include "func.h"
#include <assert.h>
//https://github.com/jesusgollonet/ofpennereasing


__todo() //most of these funcs probably don't work becase of the new way we do time as millis, some of them never worked anyway
namespace util
{
	namespace math
	{
		float Interpolator::Linear::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			return beg + (end - beg) * float(curr_time / lerp_time);
		}
		/**
		* easing in - accelerating from zero velocity
		*/
		float Interpolator::EaseInExpo::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = float(curr_time / lerp_time);
			float dist = end - beg;
			float ret = dist * pow(lerp, expo) + beg;
			return ret;
		}
		/**
		* easing out - decelerating to zero velocity
		*/
		float Interpolator::EaseOutExpo::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = float(curr_time / lerp_time);
			float dist = end - beg;
			float multi = (Odd(expo)) ? 1.0f : -1.0f;
			float ret = dist * (multi * pow(lerp - 1.0f, expo) + 1.0f) + beg;
			return ret;
		}
		/**
		* easing in/out - acceleration until halfway, then deceleration
		*/
		float Interpolator::EaseInOutExpo::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = float(curr_time / lerp_time);
			float halfway = Linear()(beg, end, 0.5f, 1.0f);
			if (lerp < 0.5f)
			{
				EaseInExpo func;
				func.expo = expo;
				return func(beg, halfway, curr_time, lerp_time / 2.0f);
			}
			else
			{
				EaseOutExpo func;
				func.expo = expo;
				return func(halfway, end, curr_time - lerp_time * 0.5f, lerp_time* 0.5f);
			}
		}
		/**
		* circular easing in - accelerating from zero velocity
		*/
		float Interpolator::EaseInCirc::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = curr_time / lerp_time;
			float dist = end - beg;
			float ret = -dist * (sqrt(1.0f - lerp * lerp) - 1.0f) + beg;
			return ret;
		}
		/**
		* circular easing out - decelerating to zero velocity
		*/
		float Interpolator::EaseOutCirc::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = curr_time / lerp_time;
			float dist = end - beg;
			lerp -= 1.0f;
			float ret = dist * sqrt(1.0f - lerp * lerp) + beg;
			return ret;
		}
		/**
		* easing in/out - acceleration until halfway, then deceleration
		*/
		float Interpolator::EaseInOutCirc::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = curr_time / lerp_time;
			float halfway = Linear()(beg, end, 0.5f, 1.0f);
			if (lerp < 0.5f)
			{
				EaseInCirc func;
				return func(beg, halfway, curr_time, lerp_time / 2.0f);
			}
			else
			{
				EaseOutCirc func;
				return func(halfway, end, curr_time - lerp_time * 0.5f, lerp_time* 0.5f);
			}
		}
		/**
		* sine easing in - accelerating from zero velocity
		*/
		float Interpolator::EaseInSine::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = curr_time / lerp_time;
			float dist = end - beg;
			float ret = -dist * cos(lerp * PI / 2.0f) + dist + beg;
			return ret;
		}
		/**
		* sine easing out - decelerating to zero velocity
		*/
		float Interpolator::EaseOutSine::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = curr_time / lerp_time;
			float dist = end - beg;
			float ret = dist * sin(lerp * PI / 2.0f) + beg;
			return ret;
		}
		/**
		* sine easing in/out - acceleration until halfway, then deceleration
		*/
		float Interpolator::EaseInOutSine::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = curr_time / lerp_time;
			float halfway = Linear()(beg, end, 0.5f, 1.0f);
			if (lerp < 0.5f)
			{
				EaseInSine func;
				return func(beg, halfway, curr_time, lerp_time / 2.0f);
			}
			else
			{
				EaseOutSine func;
				return func(halfway, end, curr_time - lerp_time * 0.5f, lerp_time* 0.5f);
			}
		}
		/**
		* circular easing in - accelerating from zero velocity
		*/
		float Interpolator::EaseInElastic::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			//TODO broken fix it
			float lerp = curr_time / lerp_time;
			float dist = end - beg;
			float p = 0.3f;
			lerp -= 1.0f;
			float ret = float(-dist * ((pow(2, -10 * lerp) * sin((lerp - p / 4.0f) * (2 * PI) / p) + 1.0f) + 1.0f) + beg);
			return ret;
		}
		float Interpolator::EaseOutElastic::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = curr_time / lerp_time;
			float dist = end - beg;
			float p = 0.3f;
			float ret = float(dist * (pow(2, -10 * lerp) * sin((lerp - p / 4.0f) * (2 * PI) / p) + 1.0f) + beg);
			return ret;
		}
		/**
		* circular easing in - accelerating from zero velocity
		*/
		float Interpolator::EaseInBack::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			//TODO refactor
			// t = curr_time, b = beg, c = end, d = lerp_time
			float t = (float)curr_time.ToDouble();
			float b = beg;
			float c = end;
			float d = (float)lerp_time.ToDouble();

			float s = 1.70158f;
			float postFix = t /= d;
			float ret = c * (postFix)*t*((s + 1)*t - s) + b;
			return ret;
		}
		/**
		* circular easing out - decelerating to zero velocity
		*/
		float Interpolator::EaseOutBack::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = curr_time / lerp_time;
			float dist = end - beg;
			float ret = dist * sin(lerp * PI / 2.0f) + beg;
			return ret;
		}
		/**
		* easing in/out - acceleration until halfway, then deceleration
		*/
		float Interpolator::EaseInOutBack::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float lerp = curr_time / lerp_time;
			float halfway = Linear()(beg, end, 0.5f, 1.0f);
			if (lerp < 0.5f)
			{
				EaseInSine func;
				return func(beg, halfway, curr_time, lerp_time / 2.0f);
			}
			else
			{
				EaseOutSine func;
				return func(halfway, end, curr_time - lerp_time * 0.5f, lerp_time* 0.5f);
			}
		}

		float Interpolator::Overshoot::operator()(float beg, float end, Time curr_time, Time lerp_time)
		{
			float asdf = lerp_time.ToDouble() * this->pass_point_ratio;
			if (curr_time < asdf)
			{
				return Linear()(beg, end, curr_time, asdf);
			}
			else
			{
				lerp_time -= asdf;
				curr_time -= asdf;
				float lerp = -pow(curr_time / lerp_time * 2 - 1.0f, 2) + 1;
				return end + lerp * (end - beg) * over_shoot_ratio;
			}
		}

		Interpolator::Interpolator(void)
			: m_pInterpFunc(nullptr)
			, m_pFinishFunc(nullptr)
			, m_fBeg(0.0f)
			, m_fLerp(0.0f)
			, m_fEnd(0.0f)
			, m_fCurrTime(0.0f)
			, m_fLerpTime(0.0f)
			, m_bIsInterpolating(false)
		{
		}

		void Interpolator::Init(const float& beg
			, const float& end
			, const float& lerp_time
			, const std::function<float(float, float, Time, Time)> interp_func
			, const std::function<void()> pFinishFunc)
		{
			assert(interp_func);

			this->m_fBeg = beg;
			this->m_fEnd = end;
			this->m_fCurrTime = 0.0f;
			this->m_fLerpTime = lerp_time;
			this->m_pInterpFunc = interp_func;
			this->m_pFinishFunc = pFinishFunc;
			this->m_bIsInterpolating = true;
			this->m_fLerp = this->m_pInterpFunc(this->m_fBeg, this->m_fEnd, this->m_fCurrTime, this->m_fLerpTime);
		}

		void Interpolator::Update(const Time& fDelta)
		{
			if (!this->m_bIsInterpolating)
				return;

			this->m_fCurrTime = Min(this->m_fCurrTime + fDelta, this->m_fLerpTime);
			this->m_fLerp = this->m_pInterpFunc(this->m_fBeg, this->m_fEnd, this->m_fCurrTime, this->m_fLerpTime);

			if (this->m_fCurrTime >= this->m_fLerpTime)
			{
				this->GotoEnd();
			}
		}

		void Interpolator::Finish(const Time& fTime)
		{
			if (fTime == 0.0f)
			{
				this->GotoEnd();
				return;
			}
			this->m_fCurrTime = fTime * (this->m_fCurrTime / this->m_fLerpTime); //Keep the curr_time to lerp_time ratio the same
			this->m_fLerpTime = fTime;
		}

		void Interpolator::Reset(void)
		{
			this->m_fBeg = 0.0f;
			this->m_fEnd = 0.0f;
			this->m_fCurrTime = 0.0f;
			this->m_fLerpTime = 0.0f;
			this->m_fLerp = 0.0f;
			this->m_pInterpFunc = nullptr;
			this->m_pFinishFunc = nullptr;
			this->m_bIsInterpolating = false;
		}

		void Interpolator::GotoEnd(void)
		{
			this->m_fCurrTime = this->m_fLerpTime;
			this->m_bIsInterpolating = false;
			if (this->m_pFinishFunc)
				this->m_pFinishFunc();
		}
	}
}