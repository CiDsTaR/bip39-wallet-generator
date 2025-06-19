/**
 * Minimal secp256k1 implementation for Windows
 * 
 * This provides a basic secp256k1 implementation using OpenSSL's EC functionality
 * when the full libsecp256k1 is not available on Windows.
 * 
 * WARNING: This is a simplified implementation for compatibility purposes.
 * For production use, prefer the full libsecp256k1 library.
 */

#pragma once

#ifdef USE_MINIMAL_SECP256K1

#include <vector>
#include <stdexcept>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>

// Compatibility constants
#define SECP256K1_EC_COMPRESSED   1
#define SECP256K1_EC_UNCOMPRESSED 0

/**
 * Minimal secp256k1 context structure
 */
struct MinimalSecp256k1Context {
    EC_GROUP* group;
    BN_CTX* bn_ctx;
    
    MinimalSecp256k1Context() {
        group = EC_GROUP_new_by_curve_name(NID_secp256k1);
        bn_ctx = BN_CTX_new();
        
        if (!group || !bn_ctx) {
            throw std::runtime_error("Failed to initialize secp256k1 context");
        }
    }
    
    ~MinimalSecp256k1Context() {
        if (group) EC_GROUP_free(group);
        if (bn_ctx) BN_CTX_free(bn_ctx);
    }
};

/**
 * Create a minimal secp256k1 context
 */
inline MinimalSecp256k1Context* minimal_secp256k1_context_create() {
    try {
        return new MinimalSecp256k1Context();
    } catch (...) {
        return nullptr;
    }
}

/**
 * Destroy a minimal secp256k1 context
 */
inline void minimal_secp256k1_context_destroy(MinimalSecp256k1Context* ctx) {
    delete ctx;
}

/**
 * Generate public key from private key
 * 
 * @param ctx secp256k1 context
 * @param private_key 32-byte private key
 * @return 65-byte uncompressed public key (04 + 32 + 32)
 */
inline std::vector<uint8_t> minimal_secp256k1_get_public_key(
    MinimalSecp256k1Context* ctx, 
    const std::vector<uint8_t>& private_key) {
    
    if (!ctx || private_key.size() != 32) {
        throw std::runtime_error("Invalid parameters for public key generation");
    }
    
    // Create BIGNUM from private key
    BIGNUM* priv_bn = BN_bin2bn(private_key.data(), 32, nullptr);
    if (!priv_bn) {
        throw std::runtime_error("Failed to create BIGNUM from private key");
    }
    
    // Create EC_POINT for public key
    EC_POINT* pub_point = EC_POINT_new(ctx->group);
    if (!pub_point) {
        BN_free(priv_bn);
        throw std::runtime_error("Failed to create EC_POINT");
    }
    
    // Calculate public key: pub = priv * G
    if (!EC_POINT_mul(ctx->group, pub_point, priv_bn, nullptr, nullptr, ctx->bn_ctx)) {
        EC_POINT_free(pub_point);
        BN_free(priv_bn);
        throw std::runtime_error("Failed to calculate public key");
    }
    
    // Convert to uncompressed format
    size_t pub_len = EC_POINT_point2oct(ctx->group, pub_point, 
                                       POINT_CONVERSION_UNCOMPRESSED,
                                       nullptr, 0, ctx->bn_ctx);
    
    if (pub_len != 65) {
        EC_POINT_free(pub_point);
        BN_free(priv_bn);
        throw std::runtime_error("Unexpected public key length");
    }
    
    std::vector<uint8_t> public_key(pub_len);
    if (!EC_POINT_point2oct(ctx->group, pub_point, 
                           POINT_CONVERSION_UNCOMPRESSED,
                           public_key.data(), pub_len, ctx->bn_ctx)) {
        EC_POINT_free(pub_point);
        BN_free(priv_bn);
        throw std::runtime_error("Failed to serialize public key");
    }
    
    // Cleanup
    EC_POINT_free(pub_point);
    BN_free(priv_bn);
    
    return public_key;
}

/**
 * Compress a public key
 * 
 * @param uncompressed_pubkey 65-byte uncompressed public key
 * @return 33-byte compressed public key
 */
inline std::vector<uint8_t> minimal_secp256k1_compress_public_key(
    const std::vector<uint8_t>& uncompressed_pubkey) {
    
    if (uncompressed_pubkey.size() != 65 || uncompressed_pubkey[0] != 0x04) {
        throw std::runtime_error("Invalid uncompressed public key format");
    }
    
    std::vector<uint8_t> compressed(33);
    
    // Copy x-coordinate
    std::copy(uncompressed_pubkey.begin() + 1, 
              uncompressed_pubkey.begin() + 33, 
              compressed.begin() + 1);
    
    // Determine y-coordinate parity
    uint8_t y_last_byte = uncompressed_pubkey[64];
    compressed[0] = (y_last_byte & 1) ? 0x03 : 0x02;
    
    return compressed;
}

/**
 * Verify that a private key is valid
 * 
 * @param private_key 32-byte private key
 * @return true if valid, false otherwise
 */
inline bool minimal_secp256k1_verify_private_key(const std::vector<uint8_t>& private_key) {
    if (private_key.size() != 32) {
        return false;
    }
    
    // Check that key is not zero
    bool all_zero = true;
    for (uint8_t byte : private_key) {
        if (byte != 0) {
            all_zero = false;
            break;
        }
    }
    if (all_zero) {
        return false;
    }
    
    // Check that key is less than the curve order
    // secp256k1 order: FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141
    const uint8_t curve_order[] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
        0xBA, 0xAE, 0xDC, 0xE6, 0xAF, 0x48, 0xA0, 0x3B,
        0xBF, 0xD2, 0x5E, 0x8C, 0xD0, 0x36, 0x41, 0x41
    };
    
    // Simple comparison (big-endian)
    for (int i = 0; i < 32; i++) {
        if (private_key[i] < curve_order[i]) {
            return true;
        } else if (private_key[i] > curve_order[i]) {
            return false;
        }
    }
    
    return false; // Equal to order, which is invalid
}

#endif // USE_MINIMAL_SECP256K1