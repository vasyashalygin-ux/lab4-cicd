#!/usr/bin/env bash
set -e

BASE_URL="http://localhost:8080"

echo "=== Integration Tests ==="

# Test GET /
echo "[1/3] Testing GET /"
RESPONSE=$(curl -s "$BASE_URL/")
if echo "$RESPONSE" | grep -q "Hello from CI/CD"; then
    echo "  PASS: GET / returned expected response: $RESPONSE"
else
    echo "  FAIL: GET / unexpected response: $RESPONSE"
    exit 1
fi

# Test GET /health
echo "[2/3] Testing GET /health"
HTTP_CODE=$(curl -s -o /dev/null -w "%{http_code}" "$BASE_URL/health")
if [ "$HTTP_CODE" = "200" ]; then
    echo "  PASS: GET /health returned HTTP 200"
else
    echo "  FAIL: GET /health returned HTTP $HTTP_CODE"
    exit 1
fi

# Test GET /metrics
echo "[3/3] Testing GET /metrics"
METRICS=$(curl -s "$BASE_URL/metrics")
if echo "$METRICS" | grep -q "http_requests_total"; then
    echo "  PASS: GET /metrics returned metrics: $METRICS"
else
    echo "  FAIL: GET /metrics unexpected response: $METRICS"
    exit 1
fi

echo "=== All integration tests passed ==="
