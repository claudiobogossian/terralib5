#include "Animation.h"
#include "ImageItem.h"

te::qt::widgets::Animation::Animation(QObject* target, const QByteArray& propertyName, QObject* parent)
  : QPropertyAnimation(target, propertyName, parent)
{
  te::qt::widgets::AnimationItem* ai = (te::qt::widgets::AnimationItem*)target;
  ai->m_animation = this;
}

te::qt::widgets::Animation::~Animation()
{
}

void te::qt::widgets::Animation::adjustDataToAnimationTemporalExtent(const te::dt::TimePeriod& period)
{
  if(m_temporalAnimationExtent != period)
    m_temporalAnimationExtent = te::dt::TimePeriod(period.getInitialTimeInstant(), period.getFinalTimeInstant());

  AnimationItem* ai = (AnimationItem*)targetObject();

  // create route points in display projection
  ai->adjustDataToAnimationTemporalExtent();

  setDataKeyValues();
}

void te::qt::widgets::Animation::setDataKeyValues()
{
  te::qt::widgets::AnimationItem* ai = static_cast<te::qt::widgets::AnimationItem*>(targetObject());
  if (ai->pixmap().isNull())
  {
    te::qt::widgets::ImageItem* ii = static_cast<te::qt::widgets::ImageItem*>(ai);
    if (ii->m_animationFiles.empty()) // out of animation time
    {
      ai->m_animationTime.clear();
      ai->m_animationRoute.clear();
      setStartValue(QPoint(0, 0));
      setEndValue(QPoint(1, 1));
      return;
    }
  }

  // total time duration 
  boost::posix_time::ptime iTime = m_temporalAnimationExtent.getInitialTimeInstant().getTimeInstant();
  boost::posix_time::ptime fTime = m_temporalAnimationExtent.getFinalTimeInstant().getTimeInstant();
  boost::posix_time::time_duration diff = fTime - iTime;
  double totalSeconds = diff.total_seconds();

  size_t size = ai->m_animationTime.size();

  // set first animation point
  setStartValue(ai->m_animationRoute[0]);

  boost::posix_time::ptime itime = ai->m_animationTime[0].getTimeInstant();
  boost::posix_time::ptime ftime = ai->m_animationTime[(int)size-1].getTimeInstant();

  // if the initial time is greater than the total initial time,
  // be stopped until the time be equal to the initial time this animation.
  // To this add a key value.
  ai->m_norInitialTime = 0;
  if(itime > iTime)
  {
    // be stopped until the beginning of the animation time
    diff = itime - iTime;
    double seconds = diff.total_seconds();

    // normalizing the time
    double t = seconds / totalSeconds;
    ai->m_norInitialTime = t;
    t *= .999999;
    setKeyValueAt(t, ai->m_animationRoute[0]);  // add a new key value
  }

  for(size_t i = 1; i < size-1; ++i)
  {
    te::dt::TimeInstant tinstant = ai->m_animationTime[(int)i]; // animation time instant
    boost::posix_time::ptime time = tinstant.getTimeInstant();
    diff = time - iTime;
    double seconds = diff.total_seconds();

    // normalizing the time
    double t = seconds / totalSeconds;
    setKeyValueAt(t, ai->m_animationRoute[(int)i]);
  }

  // if the final time is shorter than the total final time,
  // stop when reach the end of this animation
  // To this add a key value.
  ai->m_norFinalTime = 1;
  if(ftime < fTime)
  {
    // be stopped when it reaches the end of its animation
    diff = ftime - iTime;
    double seconds = diff.total_seconds();

    // normalizing the time
    double t = seconds / totalSeconds;
    ai->m_norFinalTime = t;
    setKeyValueAt(t, ai->m_animationRoute[(int)size-1]);  // add a new key value
  }

  // set last animation point
  setEndValue(ai->m_animationRoute[(int)size-1]);
}

int te::qt::widgets::Animation::getAnimationDataIndex(const double& trel)
{
  AnimationItem* ai = (AnimationItem*)targetObject();

  // this animation: initial and final time 
  boost::posix_time::ptime aiTime = m_temporalExtent.getInitialTimeInstant().getTimeInstant();
  boost::posix_time::ptime afTime = m_temporalExtent.getFinalTimeInstant().getTimeInstant();

  // temporal animation extent 
  boost::posix_time::ptime iTime = m_temporalAnimationExtent.getInitialTimeInstant().getTimeInstant();
  boost::posix_time::ptime fTime = m_temporalAnimationExtent.getFinalTimeInstant().getTimeInstant();
  boost::posix_time::time_duration diff = fTime - iTime;
  double totalSeconds = diff.total_seconds();

  // get curent time
  int secs = qRound(totalSeconds * trel);
  boost::posix_time::time_duration td = boost::posix_time::seconds(secs);
  boost::posix_time::ptime curTime = iTime + td;

  if (curTime < iTime || curTime > fTime) // out of animation
    return -1;

  size_t count = ai->m_animationTime.count();
  for (int i = 0; i < (int)count; ++i)
  {
    te::dt::TimeInstant tinstant = ai->m_animationTime[i]; // animation time instant
    boost::posix_time::ptime time = tinstant.getTimeInstant();

    if (time == curTime)
      return i;
    else if (time > curTime)
    {
      if (i == 0 || i == ((int)count - 1))
        return i;
      else
      {
        diff = time - curTime;
        unsigned long long secs = abs(diff.total_seconds());

        tinstant = ai->m_animationTime[i - 1]; // before curTime
        boost::posix_time::ptime btime = tinstant.getTimeInstant();
        diff = btime - curTime;
        unsigned long long bsecs = abs(diff.total_seconds());

        tinstant = ai->m_animationTime[i + 1]; // after curTime
        boost::posix_time::ptime atime = tinstant.getTimeInstant();
        diff = atime - curTime;
        unsigned long long asecs = abs(diff.total_seconds());

        if (secs < bsecs && secs < asecs)
          return i;
        else if(bsecs < asecs)
          return i - 1;
        else
          return i + 1;
      }
    }
  }
  return -1;
}
