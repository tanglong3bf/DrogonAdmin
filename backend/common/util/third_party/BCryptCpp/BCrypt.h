/*
 * Copyright (c) 2006 Damien Miller <djm@mindrot.org> (jBCrypt)
 * Copyright (c) 2016 Ery E. Wahyutama (C++ port)
 * =========================================================================
 * Modified by tanglong3bf (2026): Fixed compiler errors for modern C++
 */
#ifndef __BCRYPT_CPP_H__
#define __BCRYPT_CPP_H__
#include <vector>
#include <string>
#include <sstream>

#define ArraySize(Array) sizeof(Array) / sizeof(Array[0])

#ifndef byte
typedef unsigned char byte;
#endif

namespace BCryptCpp
{
class BCrypt
{
    // BCrypt parameters
    static const int GENSALT_DEFAULT_LOG2_ROUNDS = 10;
    static const int BCRYPT_SALT_LEN = 16;

    // Blowfish parameters
    static const int BLOWFISH_NUM_ROUNDS = 16;

    // Initial contents of key schedule
    static const unsigned int m_POrig[18];
    static const unsigned int m_SOrig[1024];

    // bcrypt IV: "OrpheanBeholderScryDoubt"
    static const unsigned int m_BfCryptCiphertext[6];

    // Expanded Blowfish key
    unsigned int m_P[ArraySize(m_POrig)];
    unsigned int m_S[ArraySize(m_SOrig)];
    /*!
     * \brief
     * Encode a byte array using bcrypt's slightly-modified base64
     * encoding scheme.
     *
     * \param byteArray
     * The byte array to encode.
     *
     * \param length
     * The number of bytes to encode.
     *
     * \returns
     * Base64-encoded string.
     *
     * \throws std::invalid_argument if the length is invalid.
     *
     * \remarks
     * Note that this is *not* compatible with
     * the standard MIME-base64 encoding.
     *
     */
    static std::string EncodeBase64(const std::vector<byte> &byteArray,
                                    int length);

    /*!
     * \brief
     * Look up the 3 bits base64-encoded by the specified character,
     * range-checking against conversion table.
     *
     * \param Character
     * The base64-encoded value.
     *
     * \returns
     * The decoded value of character.
     */
    static int Char64(const char Character);

    /*!
     * \brief
     * Decode a string encoded using bcrypt's base64 scheme to a
     * byte array.
     *
     * \param encodedstring
     * The string to decode.
     *
     * \param maximumBytes
     * The maximum number of bytes to decode.
     *
     * \returns
     * Array containing the decoded bytes.
     *
     * \throws std::invalid_argument if maximumBytes is invalid
     *
     * \remarks
     * Note that this is *not* compatible with
     * the standard MIME-base64 encoding.
     */
    static std::vector<byte> DecodeBase64(const std::string &encodedstring,
                                          int maximumBytes);

    /*!
     * \brief
     * Blowfish encipher a single 64-bit block encoded as
     * two 32-bit halves.
     *
     * \param blockArray
     * An array containing the two 32-bit half blocks.
     *
     * \param offset
     * The position in the array of the blocks.
     */
    void Encipher(unsigned int blockArray[], int offset);
    /*!
     * \brief
     * Cycically extract a word of key material.
     *
     * \param data
     * the vector of byte to extract the data from.
     *
     * \param offset
     * A "pointer" (as a one-entry array) to the
     * current offset into data
     *
     * \returns
     * The next word of material from data.
     */
    static unsigned int StreamToWord(const std::vector<byte> &data,
                                     int &offset);

    /*!
     * \brief
     * Initialize the Blowfish key schedule
     */
    void InitializeKey();

    /*!
     * \brief
     * Key the Blowfish cipher.
     *
     * \param keyBytes
     * An array containing the key.
     */
    void Key(const std::vector<byte> &keyBytes);

    /*!
     * \brief
     * Perform the "enhanced key schedule" step described by
     * Provos and Mazieres in "A Future-Adaptable Password Scheme"
     * http://www.openbsd.org/papers/bcrypt-paper.ps
     *
     * \param saltBytes
     * salt information.
     *
     * \param inputBytes
     * password information
     */
    void EKSKey(const std::vector<byte> &saltBytes,
                const std::vector<byte> &inputBytes);

    template <typename T>
    static T ConvertTo(const std::string &Text)
    {
        // character array as argument
        std::stringstream ss(Text);
        T result;
        return ss >> result ? result : 0;
    }

    /*!
     * \brief
     * Convert the string into vector bytes array,
     * The wstring will treat the input as bytes,
     * thus might differ depends on sizeof(wchar_t) on the OS
     *
     * \param input
     * the string to convert.
     *
     * \returns
     * The byte array of the string
     */
    static std::vector<byte> GetBytes(const std::string &input);
    static std::vector<byte> GetBytes(const std::wstring &input);

  public:
    /*!
     * \brief
     * Perform the central password hashing step in the
     * bcrypt scheme
     *
     * \param inputBytes
     * the password in byte array to hash.
     *
     * \param saltBytes
     * the binary salt to hash with the password.
     *
     * \param logRounds
     * the binary logarithm of the number
     * of rounds of hashing to apply.
     *
     * \returns
     * array containing the binary hashed password.
     *
     * \throws std::invalid_argument on invalid rounds
     * \throws std::invalid_argument on invalid salt length.
     */
    std::vector<byte> CryptRaw(const std::vector<byte> &inputBytes,
                               const std::vector<byte> &saltBytes,
                               int logRounds);

    /*!
     * \brief
     * Hash a password using the OpenBSD bcrypt scheme.
     *
     * \param input
     * the pasword to hash using default salt generation
     *
     * \returns
     * The hashed password.
     */
    static std::string HashPassword(const std::string &input);
    static std::string HashPassword(const std::wstring &input);
    /*!
     * \brief
     * Hash a password using the OpenBSD bcrypt scheme.
     *
     * \param input
     * the pasword to hash using workfactor for salt generation.
     *
     * \param workFactor
     * The log2 of the number of rounds of hashing to apply - the work
     * factor therefore increases as 2^workFactor.
     *
     * \returns
     * The hashed password.
     */
    static std::string HashPassword(const std::wstring &input, int workFactor);
    static std::string HashPassword(const std::string &input, int workFactor);
    /*!
     * \brief
     * Hash a password using the OpenBSD bcrypt scheme.
     *
     * \param input
     * The password to hash.
     *
     * \param salt
     * the salt to hash with, must conform the salt rule.
     *
     * \returns
     * The hashed password.
     *
     * \throws std::invalid_argument on invalid salt
     * \throws std::range_error on invalid salt version
     * \throws std::invalid_argument on missing salt rounds
     */
    static std::string HashPassword(const std::string &rawInput,
                                    const std::string &salt);
    static std::string HashPassword(const std::wstring &rawInput,
                                    const std::string &salt);
    static std::string HashPassword(const std::vector<byte> &rawInput,
                                    const std::string &salt);
    /*!
     * \brief
     * Salt for use with Hashpassword routine
     *
     * \param workFactor
     * The log2 of the number of rounds of hashing to apply - the work
     * factor therefore increases as 2**workFactor.
     *
     * \returns
     * A base64 encoded salt value.
     *
     * \throws std::invalid_argument on invalid salt round.
     */
    static std::string GenerateSalt(
        int workFactor = GENSALT_DEFAULT_LOG2_ROUNDS);
    /*!
     * \brief
     * Check that a plaintext password matches a previously hashed
     * one
     *
     * \param plaintext
     * the plaintext password to verify
     *
     * \param hashed
     * the previously-hashed password
     *
     * \returns
     * true if the passwords match, false otherwise
     */
    static bool CheckPassword(const std::string &plaintext,
                              const std::string &hashed);
    static bool CheckPassword(const std::wstring &plaintext,
                              const std::string &hashed);
};
}  // namespace BCryptCpp
#endif  // __BCRYPT_CPP_H__
