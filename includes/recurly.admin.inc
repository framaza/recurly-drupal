<?php

/**
 * @file
 * Recurly settings forms and administration page callbacks.
 */


/**
 * Returns the site-wide Recurly settings form.
 */
function recurly_settings_form($form, &$form_state) {
  // Add form elements to collect default account information.
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default account settings'),
    '#description' => t('Configure these settings based on your Company Settings and API Credentials settings in the Recurly administration interface.'),
    '#collapsible' => TRUE,
  );
  $form['account']['recurly_subdomain'] = array(
    '#type' => 'textfield',
    '#title' => t('Subdomain'),
    '#description' => t("The subdomain of your account excluding the -test suffix if still in Sandbox."),
    '#default_value' => variable_get('recurly_subdomain', ''),
  );
  $form['account']['recurly_hosted_payment_pages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hosted Payment Pages are enabled for this account.'),
    '#default_value' => variable_get('recurly_hosted_payment_pages', FALSE),
  );
  $form['account']['recurly_api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('API username'),
    '#default_value' => variable_get('recurly_api_username', ''),
  );
  $form['account']['recurly_api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('API password'),
    '#default_value' => variable_get('recurly_api_password', ''),
  );
  $form['account']['recurly_environment'] = array(
    '#type' => 'radios',
    '#title' => t('Environment'),
    '#description' => t('Select the environment that matches your account status.'),
    '#options' => array(
      'sandbox' => t('Sandbox'),
      'production' => t('Production'),
    ),
    '#default_value' => variable_get('recurly_environment', 'sandbox'),
  );

  // Add form elements to configure default push notification settings.
  $form['push'] = array(
    '#type' => 'fieldset',
    '#title' => t('Push notification settings'),
    '#description' => t('The username and password settings must match your Push Notifications settings in the Recurly administration interface.') . '<br />' . t('Configure your default account to use @url as its Push Notification URL.', array('@url' => url('recurly/listener', array('absolute' => TRUE)))),
    '#collapsible' => TRUE,
  );
  $form['push']['recurly_push_username'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP auth username'),
    '#default_value' => variable_get('recurly_push_username', ''),
  );
  $form['push']['recurly_push_password'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP auth password'),
    '#default_value' => variable_get('recurly_push_password', ''),
  );
  $form['push']['recurly_push_logging'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following types of push notifications'),
    '#options' => array(
      'accounts' => t('Account notifications'),
      'subscriptions' => t('Subscription notifications'),
      'payments' => t('Payment notifications'),
    ),
    '#default_value' => variable_get('recurly_push_logging', array()),
  );

  // Add form elements allowing the administrator to toggle integration options.
  $form['integration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Integration options'),
    '#collapsible' => TRUE,
  );
  $form['integration']['recurly_account_integration'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Account integration'),
    '#options' => array(
      'profile_display' => t('Display Recurly account information on user profile pages, including account management links if Hosted Payment Pages are enabled.'),
    ),
    '#default_value' => variable_get('recurly_account_integration', array()),
  );

  return system_settings_form($form);
}

/**
 * Displays a list of subscription plans currently defined in your Recurly account.
 */
function recurly_subscription_plans_overview() {
  // Initialize the Recurly client with the site-wide settings.
  if (!recurly_client_initialize()) {
    return t('Could not initialize the Recurly client.');
  }

  // Retrieve the plans for the current account.
  $plans = recurly_subscription_plans();

  // Format the plan data into a table for display.
  $header = array(t('Subscription plan'), t('Price'), t('Setup fee'), t('Trial'), t('Created'), t('Operations'));
  $rows = array();

  foreach ($plans as $plan) {
    $operations = array();
    $description = '';

    // Prepare the description string if one is given for the plan.
    if (!empty($plan->description)) {
      $description = '<div class="description">' . check_plain($plan->description) . '</div>';
    }

    // Add an edit link if available for the current user.
    if (user_access('edit subscription plans')) {
      $operations[] = array(
        'title' => t('edit'),
        'href' => recurly_subscription_plan_edit_url($plan),
      );
    }

    // Add a purchase link if Hosted Payment Pages are enabled.
    if (variable_get('recurly_hosted_payment_pages', FALSE)) {
      $operations[] = array(
        'title' => t('purchase'),
        'href' => recurly_subscription_plan_purchase_url($plan),
      );
    }

    $rows[] = array(
      t('@name <small>(Plan code: @code)</small>', array('@name' => $plan->name, '@code' => $plan->plan_code)) . $description,
      t('@unit_price per @interval_length @interval_unit', array('@unit_price' => number_format($plan->unit_amount_in_cents / 100, 2), '@interval_length' => $plan->plan_interval_length, '@interval_unit' => $plan->plan_interval_unit)),
      t('@setup_fee', array('@setup_fee' => number_format($plan->setup_fee_in_cents / 100, 2))),
      t('@trial_length @trial_unit', array('@trial_length' => $plan->trial_interval_length, '@trial_unit' => $plan->trial_interval_unit)),
      format_date($plan->created_at),
      theme('links', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline')))),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No subscription plans found.'), 'colspan' => 5));
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
