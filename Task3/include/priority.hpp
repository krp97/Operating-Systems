class Priority
{
   public:

    explicit Priority(int priority)
    {
        priority_        = static_cast<float>(priority);
        increase_factor_ = priority_ * 0.1;
    };

    Priority()                = delete;
    Priority(const Priority&) = default;
    Priority& operator=(const Priority&) = default;
    Priority(Priority&&)                 = default;
    Priority& operator=(Priority&&) = default;
    ~Priority()                     = default;

    int get_priority() const;
    void increase_priority() { priority_ += increase_factor_; };

   private:
    float priority_;
    float increase_factor_;
};