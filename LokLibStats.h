#include <vector>
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
    double m_std_dev;

public:  
    LokLibStats(){ clear(); };

    void clear()
    {
        m_min = std::numeric_limits<double>::max();
        m_max = -std::numeric_limits<double>::max();
        m_count = 0u;
        m_cached_std_dev = false;
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
    };

    size_t get_count()
    {
        return m_count;
    };

    double get_mean()
    {
        return m_sum / static_cast<double>(m_count);
    };

    double get_minimum()
    {
        return m_min;
    };

    double get_maximum()
    {
        return m_max;
    };

    double get_medium()
    {
        double medium;
        if( m_count % 2u == 0u )
        {
            medium = (m_values[m_count/2u-1] + m_values[m_count/2u]) / 2.0;
        }
        else
        {
            medium = m_values[m_count/2u];
        }
        return medium;
    };

    double calculate_std_deviation()
    {
        if( !m_cached_std_dev )
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

        return sqrt(m_std_dev);
    };
};