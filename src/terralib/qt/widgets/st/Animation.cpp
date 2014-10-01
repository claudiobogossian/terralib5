#include "Animation.h"
#include "TrajectoryItem.h"

te::qt::widgets::Animation::Animation(QObject* target, const QByteArray& propertyName, QObject* parent)
  : QPropertyAnimation(target, propertyName, parent)
{
  te::qt::widgets::AnimationItem* ai = (te::qt::widgets::AnimationItem*)target;
  ai->m_animation = this;
}

te::qt::widgets::Animation::~Animation()
{
}

void te::qt::widgets::Animation::createAnimationDataInDisplayProjection(const te::dt::TimePeriod& period)
{
  if(m_temporalAnimationExtent != period)
    m_temporalAnimationExtent = te::dt::TimePeriod(period.getInitialTimeInstant(), period.getFinalTimeInstant());

  AnimationItem* ai = (AnimationItem*)targetObject();

  // create route points in display projection
  ai->createAnimationDataInDisplayProjection();

  setDataKeyValues();
}

void te::qt::widgets::Animation::setDataKeyValues()
{
  te::qt::widgets::AnimationItem* ai = (te::qt::widgets::AnimationItem*)targetObject();

  // total time duration 
  boost::posix_time::ptime iTime = m_temporalAnimationExtent.getInitialTimeInstant().getTimeInstant();
  boost::posix_time::ptime fTime = m_temporalAnimationExtent.getFinalTimeInstant().getTimeInstant();
  boost::posix_time::time_duration diff = fTime - iTime;
  double totalSeconds = diff.total_seconds();

  size_t size = ai->m_animationTime.size();

  // set first animation point
  setStartValue(ai->m_animationRoute[0]);

  boost::posix_time::ptime itime = ai->m_animationTime[0].getTimeInstant();
  boost::posix_time::ptime ftime = ai->m_animationTime[size-1].getTimeInstant();

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
    te::dt::TimeInstant tinstant = ai->m_animationTime[i]; // animation time instant
    boost::posix_time::ptime time = tinstant.getTimeInstant();
    diff = time - iTime;
    double seconds = diff.total_seconds();

    // normalizing the time
    double t = seconds / totalSeconds;
    setKeyValueAt(t, ai->m_animationRoute[i]);
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
    setKeyValueAt(t, ai->m_animationRoute[size-1]);  // add a new key value
  }

  // set last animation point
  setEndValue(ai->m_animationRoute[size-1]);
}

int te::qt::widgets::Animation::getAnimationDataIndex(const double& trel)
{
  AnimationItem* ai = (AnimationItem*)targetObject();
  size_t size = ai->m_animationTime.size();

  // animation time duration 
  boost::posix_time::ptime aiTime = m_temporalExtent.getInitialTimeInstant().getTimeInstant();
  boost::posix_time::ptime afTime = m_temporalExtent.getFinalTimeInstant().getTimeInstant();

  // total time duration 
  boost::posix_time::ptime iTime = m_temporalAnimationExtent.getInitialTimeInstant().getTimeInstant();
  boost::posix_time::ptime fTime = m_temporalAnimationExtent.getFinalTimeInstant().getTimeInstant();
  boost::posix_time::time_duration diff = fTime - iTime;
  double totalSeconds = diff.total_seconds();

  diff = aiTime - iTime;
  double aiSeconds = diff.total_seconds();
  double itrel = aiSeconds / totalSeconds;
  if(itrel > trel)
    return 0;

  diff = afTime - iTime;
  double afSeconds = diff.total_seconds();
  double ftrel = afSeconds / totalSeconds;
  if(trel > ftrel)
    return size-1;

  size_t i;
  for(i = 0; i < size; ++i)
  {
    te::dt::TimeInstant tinstant = ai->m_animationTime[i]; // animation time instant
    boost::posix_time::ptime time = tinstant.getTimeInstant();
    diff = time - iTime;
    double seconds = diff.total_seconds();

    // normalizing the time
    double t = seconds / totalSeconds;

    if(t >= trel)
    {
      if(direction() == QAbstractAnimation::Forward && i != 0)
        return i - 1;
      else
        return i;
    }
  }
  if(i == size)
    return size - 2;
  return 0;
}
