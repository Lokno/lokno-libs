#include <vector>
#include <algorithm>
#include <functional>
#include <limits>
#include <cmath>

class LokLibStats
{
    double m_min;
    double m_max;
    double m_sum;
    size_t m_count;
    std::vector<double> m_values;

    bool m_cached_std_dev;
    bool m_cached_mean;
    double m_std_dev;
    double m_medium;

public:  
    LokLibStats(){ clear(); };

    void clear()
    {
        m_min = std::numeric_limits<double>::max();
        m_max = -std::numeric_limits<double>::max();
        m_sum = 0;
        m_count = 0u;
        m_cached_std_dev = false;
        m_cached_mean = false;
        m_std_dev = 0.0;
        m_medium = 0.0;
        m_values.clear();
    }

    void add( double value )
    {
        m_count++;
        m_sum += value;
        if( value < m_min ) m_min = value;
        if( value > m_max ) m_max = value;
        m_values.push_back(value);

        // dirty cached values
        m_cached_std_dev = false;
        m_cached_mean = false;
    };

    size_t get_count()
    {
        return m_count;
    };

    double get_mean()
    {
        double mean = 0.0;
        if( m_count > 0u )
        {
            mean = m_sum / static_cast<double>(m_count);
        }
        return mean;
    };

    double get_sum()
    {
        return m_sum;
    };

    double get_minimum()
    {
        return m_min;
    };

    double get_maximum()
    {
        return m_max;
    };

    double calculate_medium()
    {
        if( !m_cached_mean )
        {
            std::sort(m_values.begin(), m_values.end(), std::greater<double>());

            if( m_count % 2u == 0u )
            {
                m_medium = (m_values[m_count/2u-1] + m_values[m_count/2u]) / 2.0;
            }
            else
            {
                m_medium = m_values[m_count/2u];
            }
        }

        return m_medium;
    };

    double calculate_std_deviation()
    {
        if( !m_cached_std_dev )
        {
            m_std_dev = 0u;
            if( m_count > 0u)
            {
                m_std_dev = 0;
                double mean = get_mean();
                
                for( auto iter=m_values.begin(); iter!=m_values.end(); ++iter )
                {
                    double temp = *iter - mean;
                    m_std_dev += temp*temp;
                }

                m_std_dev /= static_cast<double>(m_count);
            }
        }

        return sqrt(m_std_dev);
    };
};