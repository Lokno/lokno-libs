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
    bool m_cached_histogram;
    double m_std_dev;
    double m_medium;
    
    size_t* m_bins;
    int m_num_bins;
    int m_max_bin;
    int m_height_histogram = 8;

public:  
    LokLibStats( int bins = 80 )
    { 
        clear(); 
        m_num_bins = bins;
        m_bins = new size_t[m_num_bins];
    };

    ~LokLibStats()
    {
        delete[] m_bins;
    }
    
    void clear()
    {
        m_min = std::numeric_limits<double>::max();
        m_max = -std::numeric_limits<double>::max();
        m_sum = 0;
        m_count = 0u;
        m_cached_std_dev = false;
        m_cached_mean = false;
        m_cached_histogram = false;
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
        m_cached_histogram = false;
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
    
    void print_histogram()
    {
        if( !m_cached_histogram )
        {
            m_cached_histogram = true;
            for(int i = 0; i < m_num_bins; ++i)
            {
                m_bins[i] = 0u;
            }
            m_max_bin = 0;

            double bin_step = (m_max - m_min) / static_cast<double>(m_num_bins);

            for( auto iter=m_values.begin(); iter!=m_values.end(); ++iter )
            {
                int bin = static_cast<int>((*iter - m_min) / bin_step);
                if( bin >= m_num_bins ) bin = m_num_bins-1;
                m_bins[bin]++;
                if( static_cast<int>(m_bins[bin]) > m_max_bin ) m_max_bin = static_cast<int>(m_bins[bin]);
            }
        }

        for(int i = 0; i < m_num_bins; ++i)
        {
            m_bins[i] = static_cast<size_t>(floor(static_cast<float>(m_bins[i]) / static_cast<float>(m_max_bin) * static_cast<float>(m_height_histogram)));
        }


        for(int j = m_height_histogram; j > 0; --j)
        {
            for(int i = 0; i < m_num_bins; ++i)
            {
                if( static_cast<int>(m_bins[i]) >= j ) std::cout << "*";
                else std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
};
