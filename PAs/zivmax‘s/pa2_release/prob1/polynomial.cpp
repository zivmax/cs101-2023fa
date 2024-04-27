#include <complex>
#include <iostream>
#include <numbers>
#include <vector>

using std::polar;
using std::numbers::pi;
using Coeffs = std::vector<double>;
using Roots = std::vector<std::complex<double>>;
using Root = std::complex<double>;
using Freqency = std::complex<double>;
using Freqencies = std::vector<std::complex<double>>;
using Freqencies_real = std::vector<double>;
using Signal = std::vector<std::complex<double>>;
using Signal_real = std::vector<double>;


class FFT
{
    public:
        static Freqencies compute_FFT(const Signal &input)
        {
            if (input.size() == 1) { return input; }

            Signal buffer(input);

            size_t twiddle_factor = get_twiddle_factor(input.size());

            int INV_factor = (inverse) ? -1 : 1;

            fill_zeros(buffer, twiddle_factor);

            Signal input_e;
            Signal input_o;

            for (size_t i = 0; i < twiddle_factor; ++i)
            {
                if (i % 2 == 0)
                {
                    input_e.push_back(buffer[i]);
                }
                else
                {
                    input_o.push_back(buffer[i]);
                }
            }

            Freqencies output_e = compute_FFT(input_e);
            Freqencies output_o = compute_FFT(input_o);

            Freqencies output(twiddle_factor, 0);

            // Compute the roots
            Roots roots_of_unity;
            for (size_t i = 0; i < twiddle_factor; i++)
            {
                roots_of_unity.push_back(polar(1.0, INV_factor * 2.0 * pi * i / twiddle_factor));
            }


            for (size_t k = 0; k < twiddle_factor / 2; ++k)
            {
                Root w_k = roots_of_unity[k];
                output[k] = output_e[k] + w_k * output_o[k];
                output[k + twiddle_factor / 2] = output_e[k] - w_k * output_o[k];
            }

            return output;
        }

        static Freqencies compute_FFT(const Signal_real &input)
        {
            size_t n = input.size();
            Signal complexed_input;
            for (size_t i = 0; i < n; ++i)
            {
                complexed_input.push_back(input[i]);
            }

            return compute_FFT(complexed_input);
        }

        static Signal compute_IFFT(const Freqencies &input)
        {
            inverse = true;
            Signal output = compute_FFT(input);
            inverse = false;
            // Normalize the amplitude of the output
            for (auto &&i : output)
            {
                i /= get_twiddle_factor(input.size());
            }

            return output;
        }

        static Signal compute_IFFT(const Freqencies_real &input)
        {
            size_t n = input.size();
            Signal complexed_input;
            for (size_t i = 0; i < n; ++i)
            {
                complexed_input.push_back(input[i]);
            }

            return compute_IFFT(complexed_input);
        }


        static void
        fill_zeros(Signal &siganl, size_t scaled_size)
        {
            size_t n = (scaled_size > siganl.size()) ? scaled_size - siganl.size() : 0;
            for (size_t i = 0; i < n; ++i)
            {
                siganl.push_back(0);
            }
        }

        static size_t get_twiddle_factor(size_t n)
        {
            size_t twiddle_factor = 1;
            while (twiddle_factor < n)
            {
                twiddle_factor *= 2;
            }
            return twiddle_factor;
        }

    private:
        static bool inverse;
};

bool FFT::inverse = false;

class Polynomial
{
        friend int main();

    public:
        Polynomial(const Coeffs &coeffs) : coeffs(coeffs) { }

        Polynomial(Coeffs &&coeffs) : coeffs(std::move(coeffs)) { }

        Polynomial multiply(const Polynomial &other)
        {
            size_t res_len = coeffs.size() + other.coeffs.size();
            size_t n = FFT::get_twiddle_factor(res_len);

            // Add 0s to make len(a) == len(b) == len(c)
            Coeffs buffer1(coeffs);
            Coeffs buffer2(other.coeffs);
            Polynomial::fill_zeros(buffer1, n);
            Polynomial::fill_zeros(buffer2, n);

            Freqencies a_ffted = FFT::compute_FFT(buffer1);
            Freqencies b_ffted = FFT::compute_FFT(buffer2);

            // Caculate the c_ffted
            Freqencies c_ffted(n, 0);
            for (size_t i = 0; i < n; ++i)
            {
                c_ffted[i] = a_ffted[i] * b_ffted[i];
            }

            Signal c_complexed = FFT::compute_IFFT(c_ffted);
            Coeffs c;

            for (size_t i = 0; i < res_len; ++i)
            {
                c.push_back(round(c_complexed[i].real()));
            }

            return c;
        }

        // Print the polynomial with variable x
        void print()
        {
            size_t n = coeffs.size();
            for (size_t i = n - 1; i != 0; --i)
            {
                if (coeffs[i] == 0) { continue; }

                std::cout << static_cast<int>(coeffs[i]) << "x^" << i;
                std::cout << " + ";
            }

            std::cout << coeffs[0] << std::endl;
        }

        // Print the values of the coeffs ONLY
        void print_coeffs()
        {
            size_t n = coeffs.size();
            for (size_t i = 0; i < n - 1; ++i)
            {
                std::cout << static_cast<int>(coeffs[i]) << ' ';
            }
            std::cout << std::endl;
        }

        Polynomial operator*(const Polynomial &other)
        {
            return multiply(other);
        }

    private:
        Coeffs coeffs;


        static void fill_zeros(Coeffs &coeffs, size_t scaled_size)
        {
            size_t n = (scaled_size > coeffs.size()) ? scaled_size - coeffs.size() : 0;
            for (size_t i = 0; i < n; ++i)
            {
                coeffs.push_back(0);
            }
        }
};

int main()
{
    // Get the length of poly 1 and poly 2 from first line
    size_t n1, n2;
    std::cin >> n1 >> n2;

    // Get the coeffs of poly 1
    Coeffs coeffs1;
    for (size_t i = 0; i < n1; ++i)
    {
        double temp;
        std::cin >> temp;
        coeffs1.push_back(temp);
    }

    // Get the coeffs of poly 2
    Coeffs coeffs2;
    for (size_t i = 0; i < n2; ++i)
    {
        double temp;
        std::cin >> temp;
        coeffs2.push_back(temp);
    }

    // Create the polynomials
    Polynomial poly1(coeffs1);
    Polynomial poly2(coeffs2);

    // Multiply the polynomials
    Polynomial poly3 = poly1 * poly2;

    poly3.print_coeffs();

    return 0;
}
