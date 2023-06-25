currently="good"
while [ "$currently" = "good" ]; do
    value=$(./main)
    currently=$(echo $value | grep -o good)
done
echo $value
