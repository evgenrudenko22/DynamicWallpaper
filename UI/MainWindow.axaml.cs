using Avalonia.Controls;
using UI.Tabs;

namespace UI;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
        ShowDynamicView();
    }

    public void OnDynamicClick(object? sender, Avalonia.Interactivity.RoutedEventArgs e) {
        ShowDynamicView();
    }

    public void OnVideosClick(object? sender, Avalonia.Interactivity.RoutedEventArgs e) {
        MainContent.Content = new Videos();
    }

    public void OnPluginsClick(object? sender, Avalonia.Interactivity.RoutedEventArgs e) {
        MainContent.Content = new Plugins();
    }

    private void ShowDynamicView() 
    {
        MainContent.Content = new DynamicView();
    }
}