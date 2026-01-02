# Security Policy

## Reporting a Vulnerability

If you discover a security vulnerability in Linguist, please report it by opening a public issue.

We take security seriously and will respond to valid reports as quickly as possible.

## Security Measures

This project employs several security practices:

- **Address Sanitizer (ASan)**: All builds run with AddressSanitizer enabled to detect memory safety issues
- **Undefined Behavior Sanitizer (UBSan)**: Enabled alongside ASan to catch undefined behavior
- **Static Analysis**: Microsoft Static Code Analysis is enabled on Windows builds
- **Minimal Dependencies**: The only external dependency is nlohmann/json, a well-maintained and widely-used library

## Supported Versions

Security updates will be applied to the latest version only.
